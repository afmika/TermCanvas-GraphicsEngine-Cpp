#include "TermCanvas.h"

TermCanvas::TermCanvas(uint32_t col_value, uint32_t row_value)
{
    this->init(col_value, row_value);
}

TermCanvas::TermCanvas() {
    //ctor
}

TermCanvas::~TermCanvas()
{
    //dtor
}

void TermCanvas::init(uint32_t col_value, uint32_t row_value) {
    this->col = col_value;
    this->row = row_value;

    screen = new std::vector<std::vector<char>>;

    for (uint32_t i = 0; i < row_value; i++) {
        std::vector<char> temp(col_value, '.');
        screen->push_back( temp );
    }
}

void TermCanvas::draw() const {
    for (uint32_t r = 0; r < row; r++) {
        for (uint32_t c = 0; c < col; c++) {
            std::cout << " " << (*screen)[r][c];
        }
        std::cout << '\n';
    }
}

void TermCanvas::clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif // _WIN32
    std::cout << std::flush;

    for (uint32_t r = 0; r < row; r++) {
        for (uint32_t c = 0; c < col; c++) {
            set(c, r, default_dead_cell);
        }
    }
}


bool TermCanvas::isInside(int x, int y) {
    return x >= 0 && x < (int) col &&
           y >= 0 && y < (int) row;
}

void TermCanvas::set(int x, int y, Color value ) {
    if ( isInside(x, y) ) {
        (*screen)[y][x] = value.toChar( DEFAULT_COLOR_PALETTE );
    }
}

void TermCanvas::set(int x, int y, char charact ) {
    if ( isInside(x, y) ) {
        (*screen)[y][x] = charact;
    }
}

void TermCanvas::set(int x, int y) {
    if ( isInside(x, y) ) {
        (*screen)[y][x] = default_alive_cell.toChar( DEFAULT_COLOR_PALETTE );
    }
}
void TermCanvas::setColorPalette(std::string letters) {
    DEFAULT_COLOR_PALETTE = letters;
}



void TermCanvas::line(int xi, int yi, int xf, int yf, Color value) {
    int dy = (yf - yi);
    int dx = (xf - xi);
    // vertical
    if ( dx == 0 ) {
        int dp = dy < 0 ? -1 : 1;
        do {
            set(xi, yi, value);
            yi += dp;
        } while ( yi != yf );
        return;
    }

    // horizontal
    if ( dy == 0 ) {
        int dp = dx < 0 ? -1 : 1;
        do {
            set(xi, yi, value);
            xi += dp;
        } while ( xi != xf );
        return;
    }

    // dx != 0 && dy != 0
    int incr_e  = 2 * dy ;
    int incr_ne = 2 * (dy - dx);
    int e = 2 * dy - dx;
    while( xi <= xf ) {
        set(xi, yi, value);
        if(e >= 0){
            yi += 1;
            e  += incr_ne;
        } else {
            e += incr_e;
        }
        xi++;
    }
}

void TermCanvas::line(int xi, int yi, int xf, int yf) {
    line(xi, yi, xf, yf, default_alive_cell);
}
void TermCanvas::fillRect(int x, int y, int width, int height) {
    int limit = x + width;
    while ( x != limit ) {
        line(x, y, x, y + height, fill_color);
        x++;
    }
}

void TermCanvas::strokeRect(int x, int y, int width, int height) {
    width--; height--;
    line(x, y, x + width, y, stroke_color); // top (left -> right)
    line(x + width, y, x + width, y + height, stroke_color);  // top right -> bottom right
    line(x + width, y + height, x, y + height, stroke_color); // bottom right -> bottom left
    line(x, y + height, x, y, stroke_color); // bottom left -> top right
}

void TermCanvas::strokeCircle(int x, int y, int radius) {
    int xi = radius,
        yi = 0,
        dx = 1 - 2 * radius,
        dy = 1,
        rad_err = 0;
    while( xi >= yi ) {
        std::vector<std::pair<int, int>> pts8 {
            {xi, yi},
            {-xi, -yi},
            {xi, -yi},
            {-xi, yi},
        };
        for (std::pair<int, int> pts : pts8) {
            int px = pts.first, py = pts.second;
            set(px + x, py + y, stroke_color);
            set(py + x, px + y, stroke_color);
        }
        yi++;
        rad_err += dy;
        dy += 2;
        if ( 2 * rad_err + dx > 0 ) {
            xi--;
            rad_err += dx;
            dx += 2;
        }
    }
}

void TermCanvas::fillCircle(int x, int y, int radius) {

}

void TermCanvas::plot2d(int (*f)(int), std::pair<int, int> x_range) {
    int x = x_range.first,
        x_end = x_range.second;
    int y = (*f)(x);

    while ( x < x_end ) {
        int nx = x + 1;
        int ny = (*f)(x);
        line(x, y, nx, ny, stroke_color);
        x = nx;
        y = ny;
    }
}


void TermCanvas::text(int x, int y, std::string text_value) {
    for(char c : text_value) {
        set(x++, y, c);
    }
}

void TermCanvas::setFillColor(Color color) {
    this->fill_color = color;
}

void TermCanvas::setStrokeColor(Color color) {
    this->stroke_color = color;
}

Color TermCanvas::getFillColor() const {
    return fill_color;
}
Color TermCanvas::getStrokeColor() const {
    return stroke_color;
}
