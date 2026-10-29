#include<iostream>
#include<filesystem>
#include<fstream>
#include<string_view>
#include<vector>
#include<algorithm>
#include<cctype>
#include<cstdio>
#include<cstdlib>
#include"pdfgen.h"

#define endl '\n'

typedef uint8_t     u8;
typedef uint32_t    u32;
typedef int32_t     i32;
typedef float       f32;

#define PDF_POINT_TO_MM(p) ((p) * 25.4f / 72.0f)

#define WARNING_ASSERT(x)                                                           \
    do {                                                                            \
        if (!(x)) {                                                                 \
            std::cerr<< "WARNING -> ["<<__FILE__<<"] "<<__LINE__<<": "<<#x<<endl;   \
        }                                                                           \
    } while (0)

#define FATAL_ASSERT(x)                                                             \
    do {                                                                            \
        if (!(x)) {                                                                 \
            std::cerr<< "FATAL -> ["<<__FILE__<<"] "<<__LINE__<<": "<<#x<<endl;     \
            std::abort();                                                           \
        }                                                                           \
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

void draw_topic_title(pdf_doc* pdf, pdf_object* page, std::string topic, f32 y, i32 column)
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
    draw_line(pdf, page, col_start, y + 4.5f, col_start + COL_W, y + 4.5f, muted_crimson, 0.4f);
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
    const std::vector<std::pair<int, std::string>>& lines
)
{
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

    const char* font_bold = pdf_set_font_ttf(pdf, FONT_BOLD);
    f32 width_name = 0.0f;
    WARNING_ASSERT(pdf_get_font_text_width(pdf, font_bold, slice.c_str(), TITLE_SIZE, &width_name) >= 0);

    pdf_add_filled_rectangle(
        pdf, page,
        PDF_MM_TO_POINT(x),
        PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y + title_h),
        width_name + PDF_MM_TO_POINT(4.0f),
        PDF_MM_TO_POINT(title_h),
        0.0f,
        muted_crimson.data, muted_crimson.data
    );
    write_text(pdf, page, FONT_BOLD, x + 2.0f, y + 0.5f, slice.c_str(), TITLE_SIZE, 'L', Color(255, 255, 255));

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

    for (auto& dir: topics) {
        current_topic = dir.filename().string();
        if (!page) {
            page = new_page();
        }
        std::vector<std::filesystem::path> files;
        for (const auto& entry: std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".cpp") files.push_back(entry.path());
        }
        sort(begin(files), end(files), lesser_path);

        draw_topic_title(pdf, page, current_topic, y, column);
        y += 7.0f;

        for (auto& file: files) {
            f32 code_max_w = COL_W - GUTTER_W - 2.5f;
            auto lines = read_file(pdf, file, code_max_w);

            const f32 box_h = 4.2f + (f32)lines.size() * CODE_LINE_H + 1.5f;

            if (y + box_h > BOTTOM) {
                if (column == 0) {
                    column = 1;
                    y = TOP;
                } else {
                    page = new_page();
                    column = 0;
                    y = TOP;
                }
            }

            const f32 x = HSPACE + column * (COL_W + COL_GAP);
            draw_source_box(pdf, page, x, y, file.filename().string(), lines);
            y += box_h + 3.0f;
        }
    }

    FATAL_ASSERT(pdf_save(pdf, "./../bookhand.pdf") >= 0);
    pdf_destroy(pdf);
}