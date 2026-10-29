#include<iostream>
#include<filesystem>
#include<fstream>
#include<string_view>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<cstdlib>
#include"pdfgen.h"

#define endl '\n'

typedef uint8_t     u8;
typedef uint32_t    u32;
typedef int32_t     i32;
typedef float       f32;

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
constexpr f32 RIGHT_SPACE = PAGE_W-HSPACE;

struct Color {
    u32 data;
    constexpr Color(u8 r, u8 g, u8 b) : data(PDF_RGB(r, g, b)) {};
};

constexpr Color ink{34, 34, 34};
constexpr Color muted_crimson{186, 81, 84};

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
    const char* bold_font = "font/lmroman10-bold.ttf";

    WARNING_ASSERT(write_text(pdf, page, bold_font, LEFT_SPACE, 3, "CHAKOTEAM!", 12, 'L', ink) >= 0);
    WARNING_ASSERT(write_text(pdf, page, bold_font, 104, 3, "ICPC Notebook", 12, 'C', ink) >= 0);
    WARNING_ASSERT(draw_line(pdf, page, LEFT_SPACE, 8, RIGHT_SPACE, 8, ink, 0.2f) >= 0);

    if (topic.size() == 0) return;
    std::string slice(topic.substr(3, topic.size()));
    slice[0] = std::toupper(slice[0]);

    for (i32 i = 1; i < slice.size()-1; i++) {
        if (slice[i] != '_') continue;
        slice[i] = ' ';
        slice[i+1] = std::toupper(slice[i+1]);
    }
    WARNING_ASSERT(write_text(pdf, page, bold_font, RIGHT_SPACE, 3, slice.c_str(), 12, 'R', ink) >= 0);
}

void write_footer(pdf_doc* pdf, pdf_object* page, i32 page_number)
{
    const char* bold_font = "font/lmroman10-bold.ttf";
    std::string page_str = std::to_string(page_number);

    WARNING_ASSERT(write_text(
                pdf, page, bold_font,
                PAGE_W/2.0f, PAGE_H-VSPACE-3, page_str.c_str(), 12, 'C', muted_crimson
            ) >= 0
    );
    WARNING_ASSERT(draw_line(pdf, page, LEFT_SPACE, PAGE_H-VSPACE-3, RIGHT_SPACE, PAGE_H-VSPACE-3, ink, 0.2f) >= 0);
}

constexpr f32 TOP = 14.0f;
constexpr f32 BOTTOM = 287.0f;

constexpr f32 COL_GAP = 5.0f;
constexpr f32 BOX_GAP = 4.0f;
constexpr f32 BOX_PAD = 3.0f;
constexpr f32 COL_W = (PAGE_W - 2.0f * HSPACE - COL_GAP) / 2.0f;

constexpr f32 CODE_SIZE = 9.0f;
constexpr f32 CODE_LINE_H = 3.5f;
constexpr f32 TITLE_SIZE = 9.0f;

const char* FONT_BOLD = "font/lmroman10-bold.ttf";
const char* FONT = "font/lmroman10-regular.ttf";

void draw_box(pdf_doc* pdf, pdf_object* page, f32 x, f32 y, f32 w, f32 h, Color color, f32 thickness)
{
    draw_line(pdf, page, x    , y    , x + w, y    , color, thickness);
    draw_line(pdf, page, x    , y + h, x + w, y + h, color, thickness);
    draw_line(pdf, page, x    , y    , x    , y + h, color, thickness);
    draw_line(pdf, page, x + w, y    , x + w, y + h, color, thickness);
}

void draw_topic_title(pdf_doc* pdf, pdf_object* page, std::string topic, f32 y, i32 column)
{
    const char* bold_font = "font/lmroman10-bold.ttf";

    std::string slice(topic.substr(3, topic.size()));
    slice[0] = std::toupper(slice[0]);

    for (i32 i = 1; i < slice.size()-1; i++) {
        if (slice[i] != '_') continue;
        slice[i] = ' ';
        slice[i+1] = std::toupper(slice[i+1]);
    }

    write_text(pdf, page, bold_font, LEFT_SPACE, y, slice.c_str(), 14, 'L', ink);
    draw_line(pdf, page,
            LEFT_SPACE + column * (RIGHT_SPACE - LEFT_SPACE) / 2.0f, y + VSPACE, 
            RIGHT_SPACE / 2.0f + column * (RIGHT_SPACE - LEFT_SPACE) / 2.0f, y + VSPACE,
            muted_crimson, 0.35f
    );
}

std::vector<std::string> read_file(pdf_doc* pdf, const std::filesystem::path& path, f32 max_width)
{
    std::vector<std::string> lines = {};
    std::ifstream file(path);
    WARNING_ASSERT(file.is_open());
    if (!file.is_open()) return lines;

    const char* font = pdf_set_font_ttf(pdf, FONT);
    WARNING_ASSERT(font);

    max_width = PDF_MM_TO_POINT(max_width);

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.empty()) {
            lines.emplace_back();
            continue;
        }

        std::string_view line_prime(line);
        while (!line_prime.empty()) {
            f32 width = 0.0f;
            
            std::size_t l = 1, r = line_prime.size() + 1;
            while (r-l > 1) {
                const std::size_t m = (l + r) / 2;
                const std::string slice(line_prime.substr(0, m));

                WARNING_ASSERT(pdf_get_font_text_width(pdf, font, slice.c_str(), CODE_SIZE, &width) >= 0);

                if (width > max_width) r = m;
                else l = m;
            }

            lines.emplace_back(line_prime.substr(0, l));
            line_prime.remove_prefix(l);
        }
    }

    for (auto i: lines) std::cout<<i<<endl;
    return lines;
}

void draw_source_box(
        pdf_doc* pdf, pdf_object* page,
        f32 x, f32 y,
        const std::string& filename, const std::vector<std::string>& lines
)
{
    const f32 title_h = 4.0f;
    const f32 box_h = title_h + lines.size() * CODE_LINE_H;

    const char* font_bold = pdf_set_font_ttf(pdf, FONT_BOLD);
    WARNING_ASSERT(font_bold);

    draw_box(pdf, page, x, y, COL_W, box_h, ink, 0.35f);

    f32 width_name = 0.0f;
    std::string slice(filename.substr(0, filename.size()-4));
    slice[0] = std::toupper(slice[0]);

    for (i32 i = 1; i < slice.size()-1; i++) {
        if (slice[i] != '_') continue;
        slice[i] = ' ';
        slice[i+1] = std::toupper(slice[i+1]);
    }

    WARNING_ASSERT(pdf_get_font_text_width(pdf, font_bold, slice.c_str(), TITLE_SIZE, &width_name) >= 0);
    pdf_add_filled_rectangle(
            pdf, page,
            PDF_MM_TO_POINT(x + BOX_PAD), PDF_MM_TO_POINT(PAGE_H) - PDF_MM_TO_POINT(y) - PDF_MM_TO_POINT(title_h)/2.0f,
            width_name+PDF_MM_TO_POINT(4.0f), PDF_MM_TO_POINT(title_h),
            0.0f,
            muted_crimson.data, muted_crimson.data
    );

    write_text(pdf, page, FONT_BOLD, x + BOX_PAD + 2.0f, y - title_h/2.0f, slice.c_str(), TITLE_SIZE, 'L', ink);
    
    f32 cy = y + title_h / 2.0f;

    for (const std::string& line: lines) {
        if (line.empty()) continue;

        write_text(pdf, page, FONT, x + BOX_PAD, cy, line.c_str(), CODE_SIZE, 'L', ink);

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
    //write_text(pdf, page, "font/lmroman10-bold.ttf", 105, 31, "Bookhand CHAKOTEAM!", 20, 'C', ink);

    std::vector<std::filesystem::path> topics;
    for (const auto& entry: std::filesystem::directory_iterator(std::filesystem::path("./../dsa/"))) {
        if (entry.is_directory()) topics.push_back(entry.path());
    }

    auto lesser_path = [](std::filesystem::path a, std::filesystem::path b) {return a.filename() < b.filename();};
    sort(begin(topics), end(topics), lesser_path);
    //entry.path().extension()==".cpp"
    
    i32 column = 0;
    f32 y = TOP;
    pdf_object* page = NULL;
    for(auto& dir: topics) {
        current_topic = dir.filename();
        if (!page) {
            page = new_page();
        }
        std::vector<std::filesystem::path> files;
        for (const auto& entry: std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".cpp") files.push_back(entry.path());
        }
        sort(begin(files), end(files), lesser_path);
        std::cout<<dir.filename()<<endl;

        draw_topic_title(pdf, page, current_topic, y, column);

        y += CODE_LINE_H*3;

        for (auto& file: files) {
            std::vector<std::string> lines = read_file(pdf, file, COL_W - 2.0f * BOX_PAD);

            const f32 box_h = CODE_LINE_H + 0.5f + 6.0f + lines.size() * CODE_LINE_H;

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
            y += box_h + CODE_LINE_H + 0.5f;
        }
    }

    FATAL_ASSERT(pdf_save(pdf, "./../bookhand.pdf") >= 0);
    pdf_destroy(pdf);
}
