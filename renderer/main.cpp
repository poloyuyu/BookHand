#include<iostream>
#include<filesystem>
#include<fstream>
#include<string_view>
#include<vector>
#include<algorithm>
#include<cctype>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"pdfgen.h"

#define endl '\n'

typedef uint8_t     u8;
typedef uint32_t    u32;
typedef int32_t     i32;
typedef float       f32;

#define PDF_POINT_TO_MM(p) ((p) * 25.4f / 72.0f)

#define WARNING_ASSERT(x)                                                        \
    do {                                                                         \
        if (!(x)) {                                                              \
            std::cerr<< "WARNING -> ["<<__FILE__<<"] "<<__LINE__<<": "<<#x<<endl;\
        }                                                                        \
    } while (0)

#define FATAL_ASSERT(x)                                                          \
    do {                                                                         \
        if (!(x)) {                                                              \
            std::cerr<< "FATAL -> ["<<__FILE__<<"] "<<__LINE__<<": "<<#x<<endl;  \
            std::abort();                                                        \
        }                                                                        \
    } while (0)

constexpr f32 PAGE_W = 210.0f;
constexpr f32 PAGE_H = 297.0f;

constexpr f32 HSPACE = 5.0f;
constexpr f32 VSPACE = 5.0f;
constexpr f32 LEFT_SPACE = HSPACE;
constexpr f32 RIGHT_SPACE = PAGE_W - HSPACE;

struct Color {
    u32 data;
    constexpr Color(u8 r, u8 g, u8 b) : data(PDF_RGB(r, g, b)) {};
};

// UI Colors
constexpr Color ink{34, 34, 34};
constexpr Color muted_crimson{186, 81, 84};
constexpr Color code_bg{248, 249, 250};
constexpr Color line_num_color{160, 160, 160};
constexpr Color border_color{215, 215, 220};

// Formulary UI Colors
constexpr Color form_bg{252, 251, 247};
constexpr Color form_border{218, 212, 200};
constexpr Color form_accent{45, 90, 135};
constexpr Color form_quote_bg{244, 241, 234};

// Syntax Highlighting Colors
constexpr Color syn_keyword{207, 34, 46};
constexpr Color syn_type{149, 56, 0};
constexpr Color syn_comment{106, 115, 125};
constexpr Color syn_string{10, 48, 105};
constexpr Color syn_preproc{111, 66, 193};
constexpr Color syn_number{5, 80, 174};

constexpr f32 TOP = 14.0f;
constexpr f32 BOTTOM = 287.0f;

constexpr f32 COL_GAP = 5.0f;
constexpr f32 COL_W = (PAGE_W - 2.0f * HSPACE - COL_GAP) / 2.0f;

constexpr f32 CODE_SIZE = 7.0f;
constexpr f32 CODE_LINE_H = 2.8f;
constexpr f32 TITLE_SIZE = 8.5f;
constexpr f32 GUTTER_W = 6.0f;

const char* FONT_BOLD = "font/lmroman10-bold.ttf";
const char* FONT_CODE = "font/DejaVuSansMono.ttf";

struct Token {
    std::string text;
    Color color;
};

inline bool is_identifier_char(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

inline bool is_keyword(std::string_view w) {
    static constexpr std::string_view kw[] = {
        "auto", "break", "case", "class", "const", "constexpr",
        "continue", "default", "delete", "do", "else", "for",
        "if", "inline", "namespace", "new", "operator", "override",
        "private", "protected", "public", "return", "sizeof", "static",
        "struct", "switch", "template", "typedef", "typename", "using", "virtual", "while"
    };
    return std::binary_search(std::begin(kw), std::end(kw), w);
}

inline bool is_type(std::string_view w) {
    static constexpr std::string_view tp[] = {
        "bool", "char", "deque", "double", "f32", "float",
        "i32", "int", "int32_t", "int64_t", "long", "map",
        "pair", "priority_queue", "queue", "set", "short", "size_t",
        "stack", "string", "u32", "u8", "uint32_t", "uint64_t", "uint8_t",
        "vector", "void"
    };
    return std::binary_search(std::begin(tp), std::end(tp), w);
}

std::vector<Token> tokenize_cpp(std::string_view line) {
    std::vector<Token> tokens;
    size_t i = 0;
    const size_t n = line.size();

    while (i < n) {
        if (std::isspace(static_cast<unsigned char>(line[i]))) {
            size_t start = i;
            while (i < n && std::isspace(static_cast<unsigned char>(line[i]))) i++;
            tokens.push_back({std::string(line.substr(start, i - start)), ink});
            continue;
        }

        if (line[i] == '/' && i + 1 < n && line[i + 1] == '/') {
            tokens.push_back({std::string(line.substr(i)), syn_comment});
            break;
        }

        if (line[i] == '#') {
            size_t start = i;
            while (i < n && !std::isspace(static_cast<unsigned char>(line[i]))) i++;
            tokens.push_back({std::string(line.substr(start, i - start)), syn_preproc});
            continue;
        }

        if (line[i] == '"' || line[i] == '\'') {
            char quote = line[i];
            size_t start = i++;
            while (i < n && line[i] != quote) {
                if (line[i] == '\\' && i + 1 < n) i++;
                i++;
            }
            if (i < n) i++;
            tokens.push_back({std::string(line.substr(start, i - start)), syn_string});
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(line[i]))) {
            size_t start = i;
            while (i < n && (is_identifier_char(line[i]) || line[i] == '.')) i++;
            tokens.push_back({std::string(line.substr(start, i - start)), syn_number});
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(line[i])) || line[i] == '_') {
            size_t start = i;
            while (i < n && is_identifier_char(line[i])) i++;
            std::string_view word = line.substr(start, i - start);

            if (is_keyword(word)) {
                tokens.push_back({std::string(word), syn_keyword});
            } else if (is_type(word)) {
                tokens.push_back({std::string(word), syn_type});
            } else {
                tokens.push_back({std::string(word), ink});
            }
            continue;
        }

        tokens.push_back({std::string(1, line[i]), ink});
        i++;
    }

    return tokens;
}

// ============================================================================
// FORMULARY BLOCK STRUCTURES & FUNCTIONS (LMROMAN SAFE)
// ============================================================================

enum class FormLineType : u8 {
    NORMAL,
    BULLET,
    QUOTE,
    MATH_DISPLAY
};

struct FormLine {
    FormLineType type;
    std::string text;
};

struct FormBlock {
    std::string title;
    std::vector<FormLine> lines;
};

// Converts any non-lmroman Unicode math symbols into clean lmroman-compatible text
std::string sanitize_for_lmroman(std::string_view sv) {
    static const std::pair<std::string_view, std::string_view> rep[] = {
        {"⟺", "<=>"}, {"⟹", "=>"}, {"⟷", "<->"}, {"←", "<-"}, {"→", "->"}, {"↦", "|->"},
        {"≥", ">="}, {"≤", "<="}, {"≠", "!="}, {"≈", "~="}, {"≡", "=="}, {"≅", "~="}, {"∼", "~"},
        {"∈", " in "}, {"⊆", " subset "}, {"∩", " cap "}, {"⋃", " U "}, {"⋂", " cap "}, {"∅", "empty"},
        {"∃", "exists "}, {"∤", " !| "}, {"∣", " | "}, {"⋯", "..."}, {"…", "..."}, {"⋮", ":"}, {"⋱", "..."},
        {"∘", " o "}, {"⋆", "*"}, {"∑", "sum"}, {"∏", "prod"}, {"∫", "int"}, {"∮", "oint"},
        {"⌈", "ceil("}, {"⌉", ")"}, {"⌊", "floor("}, {"⌋", ")"}, {"⟨", "<"}, {"⟩", ">"},
        {"√", "sqrt"}, {"ℕ", "N"}, {"ℤ", "Z"}, {"ℚ", "Q"}, {"ℝ", "R"}, {"ℂ", "C"}, {"𝔽", "F"}, {"𝔼", "E"},
        {"φ", "phi"}, {"ϕ", "phi"}, {"α", "alpha"}, {"β", "beta"}, {"γ", "gamma"}, {"δ", "delta"},
        {"λ", "lambda"}, {"μ", "mu"}, {"ω", "omega"}, {"π", "pi"}, {"ψ", "psi"}, {"σ", "sigma"},
        {"θ", "theta"}, {"ζ", "zeta"}, {"↓", "_dn"}, {"↑", "_up"}, {"–", "-"}, {"—", "-"},
        {"•", "-"}, {"·", "*"}, {"×", "x"}, {"ª", "a"}, {"Â", "A^"},
        {"⋃", "U"}, {"⋂", "cap"}, {"∤", "!|"}, {"∣", "|"}, {"⌈", "("}, {"⌉", ")"},
        {"⌊", "("}, {"⌋", ")"}, {"⟨", "<"}, {"⟩", ">"}, {"⋆", "*"}, {"∘", "o"}
    };

    std::string out;
    out.reserve(sv.size());
    size_t i = 0;
    while (i < sv.size()) {
        bool matched = false;
        for (const auto& [from, to] : rep) {
            if (sv.substr(i).starts_with(from)) {
                out.append(to);
                i += from.size();
                matched = true;
                break;
            }
        }
        if (matched) continue;

        unsigned char c = static_cast<unsigned char>(sv[i]);
        // Allow strict ASCII and standard Spanish ISO-8859-1 / UTF-8 accents
        if (c < 0x80) {
            out.push_back(sv[i++]);
        } else if ((c & 0xE0) == 0xC0 && i + 1 < sv.size()) {
            out.push_back(sv[i]);
            out.push_back(sv[i + 1]);
            i += 2;
        } else {
            // Drop any other unknown multi-byte characters that crash lmroman text width metrics
            i++;
            while (i < sv.size() && (static_cast<unsigned char>(sv[i]) & 0xC0) == 0x80) i++;
        }
    }
    return out;
}

// Snap byte index backwards so we never slice a 2-byte UTF-8 Spanish accent in half
inline size_t snap_utf8_boundary(std::string_view s, size_t idx) {
    if (idx >= s.size()) return s.size();
    while (idx > 0 && (static_cast<unsigned char>(s[idx]) & 0xC0) == 0x80) {
        idx--;
    }
    return idx;
}


std::vector<Token> tokenize_md(std::string_view line, FormLineType type) {
    if (type == FormLineType::MATH_DISPLAY) {
        return {{std::string(line), syn_number}};
    }

    std::vector<Token> tokens;
    size_t i = 0;
    const size_t n = line.size();

    while (i < n) {

        // Bold: **text**
        if (i + 1 < n && line[i] == '*' && line[i + 1] == '*') {
            size_t end = line.find("**", i + 2);

            if (end != std::string_view::npos) {
                tokens.push_back({
                    std::string(line.substr(i + 2, end - i - 2)),
                    muted_crimson
                });
                i = end + 2;
            } else {
                tokens.push_back({"**", ink});
                i += 2;
            }
            continue;
        }

        // Inline math: $text$
        if (line[i] == '$') {
            size_t end = line.find('$', i + 1);

            if (end != std::string_view::npos) {
                tokens.push_back({
                    std::string(line.substr(i + 1, end - i - 1)),
                    syn_number
                });
                i = end + 1;
            } else {
                tokens.push_back({"$", ink});
                ++i;
            }
            continue;
        }

        // Inline code: `text`
        if (line[i] == '`') {
            size_t end = line.find('`', i + 1);

            if (end != std::string_view::npos) {
                tokens.push_back({
                    std::string(line.substr(i + 1, end - i - 1)),
                    syn_preproc
                });
                i = end + 1;
            } else {
                tokens.push_back({"`", ink});
                ++i;
            }
            continue;
        }

        // Normal text
        size_t start = i;

        while (i < n) {
            if (line[i] == '$' || line[i] == '`')
                break;

            if (i + 1 < n &&
                line[i] == '*' && line[i + 1] == '*')
                break;

            ++i;
        }

        // Guarantee progress.
        if (i == start) {
            ++i;
        }

        Color base_col =
            (type == FormLineType::QUOTE) ? form_accent : ink;

        tokens.push_back({
            std::string(line.substr(start, i - start)),
            base_col
        });
    }

    return tokens;
}


std::string strip_md_markers(std::string_view s) {
    std::string res;
    res.reserve(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        if (i + 1 < s.size() && s[i] == '*' && s[i + 1] == '*') {
            i++;
            continue;
        }
        if (s[i] == '$' || s[i] == '`') continue;
        res.push_back(s[i]);
    }
    return res;
}

std::vector<FormBlock> read_formulary_file(pdf_doc* pdf, const std::filesystem::path& path, f32 max_width) {
    std::vector<FormBlock> blocks;
    std::ifstream file(path);
    WARNING_ASSERT(file.is_open());
    if (!file.is_open()) return blocks;

    // Use ONLY FONT_BOLD (lmroman10-bold.ttf) as requested
    const char* font = pdf_set_font_ttf(pdf, FONT_BOLD);
    WARNING_ASSERT(font);
    f32 max_width_pt = PDF_MM_TO_POINT(max_width);

    FormBlock current_block;
    std::string stem = path.stem().string();
    current_block.title = sanitize_for_lmroman((stem.size() > 3 && stem[2] == '_') ? stem.substr(3) : stem);

    auto flush_block = [&]() {
        while (!current_block.lines.empty() && current_block.lines.back().text.empty()) {
            current_block.lines.pop_back();
        }
        if (!current_block.lines.empty()) {
            blocks.push_back(current_block);
            current_block.lines.clear();
        }
    };

    auto push_wrapped = [&](FormLineType type, std::string_view raw_sv) {
        std::string sanitized = sanitize_for_lmroman(raw_sv);
        std::string_view text_sv(sanitized);

        if (text_sv.empty()) {
            if (!current_block.lines.empty() && !current_block.lines.back().text.empty()) {
                current_block.lines.push_back({FormLineType::NORMAL, ""});
            }
            return;
        }

        bool first_chunk = true;
        while (!text_sv.empty()) {
            std::size_t l = 0, r = text_sv.size() + 1;
            while (r - l > 1) {
                const std::size_t m = (l + r) / 2;
                const std::size_t safe_m = std::max<std::size_t>(1, snap_utf8_boundary(text_sv, m));
                const std::string clean_slice = strip_md_markers(text_sv.substr(0, safe_m));

                f32 width = 0.0f;
                int status = pdf_get_font_text_width(pdf, font, clean_slice.c_str(), CODE_SIZE, &width);
                if (status < 0) {
                    // Fallback estimate if font lookup fails on a character
                    width = clean_slice.size() * (CODE_SIZE * 0.52f);
                }

                if (width > max_width_pt) r = m;
                else l = m;
            }

            std::size_t cut = snap_utf8_boundary(text_sv, l);
            if (cut == 0) {
                // Guarantee forward progress of at least 1 full UTF-8 character (prevents infinite loops!)
                unsigned char c = static_cast<unsigned char>(text_sv[0]);
                cut = (c >= 0xC0 && text_sv.size() >= 2) ? 2 : 1;
            }

            if (cut < text_sv.size()) {
                const std::size_t last_space =
                    text_sv.substr(0, cut).find_last_of(' ');

                if (last_space != std::string_view::npos &&
                    last_space > 0 &&
                    last_space > cut / 3) {
                    cut = last_space;
                }
            }

            // Guarantee progress.
            if (cut == 0) {
                unsigned char c = static_cast<unsigned char>(text_sv[0]);

                cut = (c >= 0xC0 && text_sv.size() >= 2) ? 2 : 1;
            }

            std::string chunk_str(text_sv.substr(0, cut));
            if (type == FormLineType::BULLET) {
                chunk_str = (first_chunk ? "- " : "  ") + chunk_str;
            }
            
            if (cut == 0) cut = 1;
            current_block.lines.push_back({type, chunk_str});
            first_chunk = false;
            text_sv.remove_prefix(cut);
            while (!text_sv.empty() && text_sv.front() == ' ') text_sv.remove_prefix(1);
        }
    };

    std::string raw_line;
    bool in_display_math = false;
    std::string math_accum;

    while (std::getline(file, raw_line)) {
        if (!raw_line.empty() && raw_line.back() == '\r') raw_line.pop_back();

        std::string_view sv(raw_line);
        while (!sv.empty() && sv.front() == ' ') sv.remove_prefix(1);

        if (sv.starts_with("---")) continue;

        if (sv.starts_with("## ")) {
            if (current_block.lines.empty()) {
                current_block.title = sanitize_for_lmroman(sv.substr(3));
            }
            continue;
        }

        if (sv.starts_with("### ")) {
            flush_block();
            current_block.title = sanitize_for_lmroman(sv.substr(4));
            continue;
        }

        if (sv.starts_with("$$")) {
            if (sv.size() > 4 && sv.ends_with("$$")) {
                std::string_view inner = sv.substr(2, sv.size() - 4);
                while (!inner.empty() && inner.front() == ' ') inner.remove_prefix(1);
                push_wrapped(FormLineType::MATH_DISPLAY, inner);
            } else if (!in_display_math) {
                in_display_math = true;
                math_accum = std::string(sv.substr(2));
            } else {
                in_display_math = false;
                push_wrapped(FormLineType::MATH_DISPLAY, math_accum);
                math_accum.clear();
            }
            continue;
        }
        if (in_display_math) {
            if (sv.ends_with("$$")) {
                math_accum += " " + std::string(sv.substr(0, sv.size() - 2));
                in_display_math = false;
                push_wrapped(FormLineType::MATH_DISPLAY, math_accum);
                math_accum.clear();
            } else {
                math_accum += " " + std::string(sv);
            }
            continue;
        }

        if (sv.starts_with("> ")) {
            push_wrapped(FormLineType::QUOTE, sv.substr(2));
        } else if (sv.starts_with("- ")) {
            std::cerr << "[MD] bullet: " << sv << endl;
            push_wrapped(FormLineType::BULLET, sv.substr(2));
        } else {
            std::cerr << "[MD] normal: " << sv << endl;
            push_wrapped(FormLineType::NORMAL, sv);
        }
    }

    flush_block();
    return blocks;
}

// ============================================================================
// ORIGINAL RENDERING FUNCTIONS + DRAW_FORMULARY_BOX
// ============================================================================

i32 write_text(
    pdf_doc* pdf, pdf_object* page,
    const char* font_path,
    f32 x, f32 y,
    const char* text, const f32 size_pt,
    char alignment,
    Color color
)
{
    const char* font_name = pdf_set_font_ttf(pdf, font_path);
    WARNING_ASSERT(font_name);
    if (!font_name) return -1;

    f32 text_width = 0;
    WARNING_ASSERT(pdf_get_font_text_width(pdf, font_name, text, size_pt, &text_width) >= 0);

    f32 xp = PDF_MM_TO_POINT(x);
    if (alignment == 'C') {
        xp -= text_width / 2.0f;
    } else if (alignment == 'R') {
        xp -= text_width;
    }

    f32 yp = PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y) - size_pt;
    WARNING_ASSERT(pdf_add_text(pdf, page, text, size_pt, xp, yp, color.data) >= 0);

    return 0;
}

i32 draw_line(
    pdf_doc* pdf, pdf_object* page,
    f32 x1, f32 y1,
    f32 x2, f32 y2,
    Color color,
    f32 thickness
)
{
    return pdf_add_line(
        pdf,
        page,
        PDF_MM_TO_POINT(x1), PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y1),
        PDF_MM_TO_POINT(x2), PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y2),
        PDF_MM_TO_POINT(thickness),
        color.data
    );
}

void write_header(pdf_doc* pdf, pdf_object* page, std::string topic)
{
    WARNING_ASSERT(write_text(pdf, page, FONT_BOLD, LEFT_SPACE, 3, "CHAKOTEAM!", 11, 'L', ink) >= 0);
    WARNING_ASSERT(write_text(pdf, page, FONT_BOLD, 105, 3, "ICPC Notebook", 11, 'C', ink) >= 0);
    WARNING_ASSERT(draw_line(pdf, page, LEFT_SPACE, 7.5f, RIGHT_SPACE, 7.5f, ink, 0.2f) >= 0);

    if (topic.empty()) return;
    std::string slice = (topic.size() > 3) ? topic.substr(3) : topic;
    slice[0] = std::toupper(slice[0]);

    for (size_t i = 1; i + 1 < slice.size(); i++) {
        if (slice[i] == '_') {
            slice[i] = ' ';
            slice[i+1] = std::toupper(slice[i+1]);
        }
    }
    WARNING_ASSERT(write_text(pdf, page, FONT_BOLD, RIGHT_SPACE, 3, slice.c_str(), 11, 'R', ink) >= 0);
}

void write_footer(pdf_doc* pdf, pdf_object* page, i32 page_number)
{
    std::string page_str = std::to_string(page_number);
    WARNING_ASSERT(write_text(
        pdf, page, FONT_BOLD,
        PAGE_W / 2.0f, PAGE_H - VSPACE - 3, page_str.c_str(), 10, 'C', muted_crimson
    ) >= 0);
    WARNING_ASSERT(draw_line(pdf, page, LEFT_SPACE, PAGE_H - VSPACE - 4, RIGHT_SPACE, PAGE_H - VSPACE - 4, ink, 0.2f) >= 0);
}

void draw_topic_title(pdf_doc* pdf, pdf_object* page, std::string& topic, f32 y, i32 column)
{
    std::string slice = (topic.size() > 3) ? topic.substr(3) : topic;
    slice[0] = std::toupper(slice[0]);

    for (size_t i = 1; i + 1 < slice.size(); i++) {
        if (slice[i] == '_') {
            slice[i] = ' ';
            slice[i+1] = std::toupper(slice[i+1]);
        }
    }

    f32 col_start = HSPACE + column * (COL_W + COL_GAP);
    write_text(pdf, page, FONT_BOLD, col_start, y, slice.c_str(), 12, 'L', ink);
    draw_line(pdf, page, col_start, y + 5.0f, col_start + COL_W, y + 5.0f, muted_crimson, 0.35f);
}

std::vector<std::pair<int, std::string>> read_file(pdf_doc* pdf, const std::filesystem::path& path, f32 max_width)
{
    std::vector<std::pair<int, std::string>> lines;
    std::ifstream file(path);
    WARNING_ASSERT(file.is_open());
    if (!file.is_open()) return lines;

    const char* font = pdf_set_font_ttf(pdf, FONT_CODE);
    WARNING_ASSERT(font);
    f32 max_width_pt = PDF_MM_TO_POINT(max_width);

    std::string line;
    int line_idx = 1;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::string expanded;
        for (char c : line) {
            if (c == '\t') expanded.append("    ");
            else expanded.push_back(c);
        }

        if (expanded.empty()) {
            lines.emplace_back(line_idx++, "");
            continue;
        }

        std::string_view line_prime(expanded);
        bool first_chunk = true;
        while (!line_prime.empty()) {
            f32 width = 0.0f;
            std::size_t l = 1, r = line_prime.size() + 1;
            while (r - l > 1) {
                const std::size_t m = (l + r) / 2;
                const std::string slice(line_prime.substr(0, m));
                WARNING_ASSERT(pdf_get_font_text_width(pdf, font, slice.c_str(), CODE_SIZE, &width) >= 0);
                if (width > max_width_pt) r = m;
                else l = m;
            }

            lines.emplace_back(first_chunk ? line_idx : 0, std::string(line_prime.substr(0, l)));
            first_chunk = false;
            line_prime.remove_prefix(l);
        }
        line_idx++;
    }
    return lines;
}

void draw_source_box(
    pdf_doc* pdf, pdf_object* page,
    f32 x, f32 y,
    const std::string& filename,
    const std::vector<std::pair<int, std::string>>& lines,
    bool is_continuation = false
)
{
    y += 1.0f;
    const f32 title_h = 4.2f;
    const f32 box_h = title_h + (f32)lines.size() * CODE_LINE_H + 1.5f;

    // 1. Fondo del bloque de código
    pdf_add_filled_rectangle(
        pdf, page,
        PDF_MM_TO_POINT(x),
        PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y + box_h),
        PDF_MM_TO_POINT(COL_W),
        PDF_MM_TO_POINT(box_h),
        0.0f,
        code_bg.data, code_bg.data
    );

    // 2. Bordes sutiles
    draw_line(pdf, page, x, y, x + COL_W, y, border_color, 0.2f);
    draw_line(pdf, page, x, y + box_h, x + COL_W, y + box_h, border_color, 0.2f);
    draw_line(pdf, page, x, y, x, y + box_h, border_color, 0.2f);
    draw_line(pdf, page, x + COL_W, y, x + COL_W, y + box_h, border_color, 0.2f);

    // 3. Separador vertical del gutter de numeración
    draw_line(pdf, page, x + GUTTER_W, y + title_h, x + GUTTER_W, y + box_h, border_color, 0.15f);

    // 4. Pestaña superior del título
    std::string slice = (filename.size() > 4) ? filename.substr(0, filename.size() - 4) : filename;
    slice[0] = std::toupper(slice[0]);
    for (size_t i = 1; i + 1 < slice.size(); i++) {
        if (slice[i] == '_') {
            slice[i] = ' ';
            slice[i+1] = std::toupper(slice[i+1]);
        }
    }
    if (is_continuation) {
        slice += " (cont.)";
    }

    const char* font_bold = pdf_set_font_ttf(pdf, FONT_BOLD);
    f32 width_name = 0.0f;
    WARNING_ASSERT(pdf_get_font_text_width(pdf, font_bold, slice.c_str(), TITLE_SIZE, &width_name) >= 0);

    pdf_add_filled_rectangle(
        pdf, page,
        PDF_MM_TO_POINT(x + GUTTER_W),
        PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y + title_h / 2.0f),
        width_name + PDF_MM_TO_POINT(4.0f),
        PDF_MM_TO_POINT(title_h),
        0.0f,
        muted_crimson.data, muted_crimson.data
    );
    write_text(pdf, page, FONT_BOLD,
        x + GUTTER_W + 2.0f, y - title_h / 2.0f,
        slice.c_str(), TITLE_SIZE, 'L', Color(255, 255, 255)
    );

    // 5. Renderizado de líneas y tokens coloreados
    f32 cy = y + title_h + 1.0f;
    const char* font_code = pdf_set_font_ttf(pdf, FONT_CODE);

    for (const auto& [ln, text] : lines) {
        if (ln > 0) {
            std::string num_str = std::to_string(ln);
            write_text(pdf, page, FONT_CODE, x + GUTTER_W - 1.2f, cy, num_str.c_str(), CODE_SIZE - 1.0f, 'R', line_num_color);
        }

        if (!text.empty()) {
            f32 cur_x = x + GUTTER_W + 1.2f;
            auto tokens = tokenize_cpp(text);

            for (const auto& token : tokens) {
                write_text(pdf, page, FONT_CODE, cur_x, cy, token.text.c_str(), CODE_SIZE, 'L', token.color);

                f32 pt_width = 0.0f;
                pdf_get_font_text_width(pdf, font_code, token.text.c_str(), CODE_SIZE, &pt_width);
                cur_x += PDF_POINT_TO_MM(pt_width);
            }
        }
        cy += CODE_LINE_H;
    }
}

void draw_formulary_box(
    pdf_doc* pdf, pdf_object* page,
    f32 x, f32 y,
    const std::string& block_title,
    const std::vector<FormLine>& lines,
    bool is_continuation = false
)
{
    y += 1.0f;
    const f32 title_h = 4.2f;
    const f32 box_h = title_h + (f32)lines.size() * CODE_LINE_H + 1.5f;

    // 1. Fondo crema suave para bloque de formulario (sin gutter)
    pdf_add_filled_rectangle(
        pdf, page,
        PDF_MM_TO_POINT(x),
        PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y + box_h),
        PDF_MM_TO_POINT(COL_W),
        PDF_MM_TO_POINT(box_h),
        0.0f,
        form_bg.data, form_bg.data
    );

    // 2. Bordes y barra lateral izquierda de acento
    draw_line(pdf, page, x, y, x + COL_W, y, form_border, 0.2f);
    draw_line(pdf, page, x, y + box_h, x + COL_W, y + box_h, form_border, 0.2f);
    draw_line(pdf, page, x + COL_W, y, x + COL_W, y + box_h, form_border, 0.2f);
    draw_line(pdf, page, x, y, x, y + box_h, form_accent, 0.45f);

    // 3. Pestaña superior del sub-bloque de formulario
    std::string slice = block_title;
    if (!slice.empty()) slice[0] = std::toupper(slice[0]);
    for (size_t i = 1; i + 1 < slice.size(); i++) {
        if (slice[i] == '_') {
            slice[i] = ' ';
            slice[i + 1] = std::toupper(slice[i + 1]);
        }
    }
    if (is_continuation) {
        slice += " (cont.)";
    }

    const char* font_bold = pdf_set_font_ttf(pdf, FONT_BOLD);
    f32 width_name = 0.0f;
    WARNING_ASSERT(pdf_get_font_text_width(pdf, font_bold, slice.c_str(), TITLE_SIZE, &width_name) >= 0);

    pdf_add_filled_rectangle(
        pdf, page,
        PDF_MM_TO_POINT(x + 2.0f),
        PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y + title_h / 2.0f),
        width_name + PDF_MM_TO_POINT(4.0f),
        PDF_MM_TO_POINT(title_h),
        0.0f,
        form_accent.data, form_accent.data
    );
    write_text(pdf, page, FONT_BOLD,
        x + 4.0f, y - title_h / 2.0f,
        slice.c_str(), TITLE_SIZE, 'L', Color(255, 255, 255)
    );

    // 4. Renderizado de líneas usando únicamente FONT_BOLD (lmroman)
    f32 cy = y + title_h + 1.0f;

    for (const auto& fline : lines) {
        if (!fline.text.empty()) {
            f32 cur_x = x + 1.8f;

            if (fline.type == FormLineType::QUOTE) {
                pdf_add_filled_rectangle(
                    pdf, page,
                    PDF_MM_TO_POINT(x + 1.2f),
                    PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(cy + CODE_LINE_H - 0.3f),
                    PDF_MM_TO_POINT(COL_W - 2.4f),
                    PDF_MM_TO_POINT(CODE_LINE_H),
                    0.0f,
                    form_quote_bg.data, form_quote_bg.data
                );
                draw_line(pdf, page, x + 1.2f, cy - 0.3f, x + 1.2f, cy + CODE_LINE_H - 0.3f, muted_crimson, 0.35f);
                cur_x += 1.2f;
            } else if (fline.type == FormLineType::MATH_DISPLAY) {
                cur_x += 2.5f;
            }

            auto tokens = tokenize_md(fline.text, fline.type);
            for (const auto& token : tokens) {
                if (token.text.empty()) continue;
                write_text(pdf, page, FONT_BOLD, cur_x, cy, token.text.c_str(), CODE_SIZE, 'L', token.color);

                f32 pt_width = 0.0f;
                pdf_get_font_text_width(pdf, font_bold, token.text.c_str(), CODE_SIZE, &pt_width);
                cur_x += PDF_POINT_TO_MM(pt_width);
            }
        }
        cy += CODE_LINE_H;
    }
}

void draw_index_entry(
    pdf_doc* pdf,
    pdf_object* page,
    const std::string& text,
    int page_number,
    f32& y,
    int column,
    bool is_topic
)
{
    f32 x = HSPACE + column * (COL_W + COL_GAP);

    std::string slice = (text.size() > 4) ? text.substr(0, text.size()) : text;
    slice[0] = std::toupper(slice[0]);
    for (size_t i = 1; i + 1 < slice.size(); i++) {
        if (slice[i] == '_') {
            slice[i] = ' ';
            slice[i+1] = std::toupper(slice[i+1]);
        }
    }

    if (is_topic) {
        y += 2.0f;

        write_text(
            pdf, page, FONT_BOLD,
            x, y, slice.c_str(),
            9, 'L', muted_crimson
        );

        y += 5.0f;
    } else {
        write_text(
            pdf, page, FONT_BOLD,
            x + 2.0f, y, slice.c_str(),
            7, 'L', ink
        );

        std::string number = std::to_string(page_number);

        write_text(
            pdf, page, FONT_BOLD,
            x + COL_W - 2.0f, y,
            number.c_str(), 7, 'R', muted_crimson
        );

        y += 4.0f;
    }
}

signed main()
{
    pdf_info info{};
    pdf_doc* pdf = pdf_create(PDF_MM_TO_POINT(PAGE_W), PDF_MM_TO_POINT(PAGE_H), &info);
    FATAL_ASSERT(pdf);

    i32 page_number = 1;
    std::string current_topic = "00_index";

    auto new_page = [&]() -> pdf_object* {
        pdf_object* p = pdf_append_page(pdf);
        WARNING_ASSERT(p);

        write_header(pdf, p, current_topic);
        write_footer(pdf, p, page_number);

        page_number++;
        return p;
    };

    pdf_object* front_page = new_page();

    write_text(
        pdf, front_page, FONT_BOLD,
        PAGE_W / 2.0f, TOP,
        "CHAKOTEAM! BOOKHAND", 16, 'C', ink
    );

    f32 index_y[2] = {TOP + 12.0f, TOP + 12.0f};
    int index_column = 0;

    std::vector<std::filesystem::path> topics;
    for (const auto& entry: std::filesystem::directory_iterator(std::filesystem::path("./../dsa/"))) {
        if (entry.is_directory()) topics.push_back(entry.path());
    }

    auto lesser_path = [](const std::filesystem::path& a, const std::filesystem::path& b) {
        return a.filename() < b.filename();
    };
    sort(begin(topics), end(topics), lesser_path);

    i32 column = 0;
    f32 y = TOP;
    pdf_object* page = NULL;

    auto advance_column = [&]() {
        if (column == 0) {
            column = 1;
            y = TOP;
        } else {
            page = new_page();
            column = 0;
            y = TOP;
        }

        std::cerr << "[PAGE DEBUG] page="
                  << page_number - 1
                  << " column=" << column
                  << " y=" << y << endl;
    };

    // Safe index helper so index_column never overflows past column 1
    auto safe_add_index = [&](const std::string& label, int pnum, bool is_top) {
        if (index_column >= 2) return;
        draw_index_entry(pdf, front_page, label, pnum, index_y[index_column], index_column, is_top);
        if (index_y[index_column] > BOTTOM - 10.0f) {
            index_column++;
        }
    };

    for (auto& dir: topics) {
        current_topic = dir.filename().string();
        if (!page) {
            page = new_page();
        }
        std::vector<std::filesystem::path> files;
        for (const auto& entry: std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension();
                if (ext == ".cpp" || ext == ".md") files.push_back(entry.path());
            }
        }
        sort(begin(files), end(files), lesser_path);

        if (y + 12.0f > BOTTOM) {
            advance_column();
        }

        draw_topic_title(pdf, page, current_topic, y, column);
        safe_add_index(current_topic.substr(3), 0, true);

        y += 7.0f;

        for (auto& file: files) {
            const f32 title_h = 4.2f;
            const f32 padding = 1.5f + 1.0f;

            // Branch 1: Formulary (.md) files -> rendered with FONT_BOLD (lmroman)
            if (file.extension() == ".md") {
                f32 form_max_w = COL_W - 4.5f;
                auto blocks = read_formulary_file(pdf, file, form_max_w);
                if (blocks.empty()) continue;

                safe_add_index(file.stem().string(), page_number - 1, false);
                std::cout << "procesando formulario " << file.filename().string() << endl;

                for (const auto& block : blocks) {
                    size_t line_offset = 0;
                    bool is_continuation = false;

                    while (line_offset < block.lines.size()) {
                        // Move to another column/page if there isn't enough room.
                        const f32 min_required =
                            title_h + padding + CODE_LINE_H;

                        if (BOTTOM - y < min_required) {
                            advance_column();
                        }

                        const f32 available_h = BOTTOM - y;
                        const f32 h_for_lines = available_h - title_h - padding;

                        // Calculate how many complete lines fit.
                        const size_t can_fit = static_cast<size_t>(
                            std::max(0.0f, std::floor(h_for_lines / CODE_LINE_H))
                        );

                        const size_t remaining = block.lines.size() - line_offset;
                        const size_t count = std::min(can_fit, remaining);

                        // Defensive check: never allow an infinite pagination loop.
                        if (count == 0) {
                            std::cerr << "ERROR: Cannot fit formulary block: "
                                << block.title << endl;
                            std::cerr << "Available height: " << available_h << endl;
                            std::cerr << "Current y: " << y << endl;

                            // Force a fresh page if necessary.
                            page = new_page();
                            column = 0;
                            y = TOP;

                            // If even a fresh page cannot fit one line, stop.
                            if (BOTTOM - TOP < min_required) {
                                FATAL_ASSERT(false);
                            }

                            continue;
                        }

                        std::vector<FormLine> chunk(
                            block.lines.begin() + line_offset,
                            block.lines.begin() + line_offset + count
                        );

                        const f32 x = HSPACE + column * (COL_W + COL_GAP);

                        const f32 chunk_box_h =
                            title_h + static_cast<f32>(chunk.size()) * CODE_LINE_H + 1.5f;

                        draw_formulary_box(
                            pdf, page, x, y,
                            block.title, chunk, is_continuation
                        );

                        y += chunk_box_h + 3.0f;

                        line_offset += count;
                        is_continuation = true;

                        if (line_offset < block.lines.size()) {
                            advance_column();
                        }
                    }
                }
                continue;
            }

            // Branch 2: Original C++ (.cpp) files
            f32 code_max_w = COL_W - GUTTER_W - 2.5f;
            auto lines = read_file(pdf, file, code_max_w);
            if (lines.empty()) continue;

            safe_add_index(file.stem().string(), page_number - 1, false);
            std::cout << "procesando " << file.filename().string() << endl;

            size_t line_offset = 0;
            bool is_continuation = false;

            while (line_offset < lines.size()) {
                f32 available_h = BOTTOM - y;

                if (available_h < title_h + padding + 2.0f * CODE_LINE_H) {
                    advance_column();
                    available_h = BOTTOM - y;
                }

                f32 h_for_lines = available_h - title_h - padding;
                size_t can_fit = static_cast<size_t>(std::max(0.0f, h_for_lines / CODE_LINE_H));
                size_t count = std::min(can_fit, lines.size() - line_offset);

                if (count == 0) {
                    advance_column();
                    continue;
                }

                std::vector<std::pair<int, std::string>> chunk(
                    lines.begin() + line_offset,
                    lines.begin() + line_offset + count
                );

                const f32 x = HSPACE + column * (COL_W + COL_GAP);
                const f32 chunk_box_h = title_h + (f32)chunk.size() * CODE_LINE_H + 1.5f;

                draw_source_box(pdf, page, x, y, file.filename().string(), chunk, is_continuation);

                y += chunk_box_h + 3.0f;
                line_offset += count;
                is_continuation = true;

                if (line_offset < lines.size()) {
                    advance_column();
                }
            }
        }
    }

    FATAL_ASSERT(pdf_save(pdf, "./../bookhand.pdf") >= 0);
    pdf_destroy(pdf);
}
