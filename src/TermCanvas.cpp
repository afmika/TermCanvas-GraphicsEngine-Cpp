#include "TermCanvas.h"

void drawArc(TermCanvas* canvas, int x, int y, int radius, Color color) {
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
            canvas->set(px + x, py + y, color);
            canvas->set(py + x, px + y, color);
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
            set(c - translate_x, r - translate_y, default_dead_cell);
        }
    }
}


bool TermCanvas::isInside(int x, int y) {
    return x >= 0 && x < (int) col &&
           y >= 0 && y < (int) row;
}

void TermCanvas::set(int x, int y, Color value ) {
    x += translate_x;
    y += translate_y;
    if ( isInside(x, y) ) {
        (*screen)[y][x] = value.toChar( DEFAULT_COLOR_PALETTE );
    }
}

void TermCanvas::set(int x, int y, char charact ) {
    x += translate_x;
    y += translate_y;
    if ( isInside(x, y) ) {
        (*screen)[y][x] = charact;
    }
}

void TermCanvas::set(int x, int y) {
    x += translate_x;
    y += translate_y;
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
    if ( dx == 0 && dy != 0) {
        int dp = dy < 0 ? -1 : 1;
        do {
            set(xi, yi, value);
            yi += dp;
        } while ( yi != yf );
        return;
    }

    // horizontal
    if ( dy == 0 && dx != 0) {
        int dp = dx < 0 ? -1 : 1;
        do {
            set(xi, yi, value);
            xi += dp;
        } while ( xi != xf );
        return;
    }

    if ( dx == 0 && dy == 0 ) {
        return;
    }

    // dx != 0 && dy != 0
    // Bresenham's algorithm
    dx = abs(dx);
    dy = abs(dy);
    int sx = xi < xf ? 1 : -1;
    int sy = yi < yf ? 1 : -1;
    int err = dx - dy;
    while ( true ) {
        set(xi, yi, value);
        if ( xi == xf && yi == yf ) {
            break;
        }
        int e2 = 2 * err;
        if ( e2 > -dy ) {
            err -= dy;
            xi += sx;
        }
        if ( e2 < dx ) {
            err += dx;
            yi += sy;
        }
    }
}

void TermCanvas::line(int xi, int yi, int xf, int yf) {
    line(xi, yi, xf, yf, stroke_color);
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
    TermCanvas* canvas = this;
    drawArc(canvas, x, y, radius, stroke_color);
}

void TermCanvas::fillCircle(int x, int y, int radius) {
    TermCanvas* canvas = this;
    for (int r = 0; r <= radius; r++ )
        drawArc(canvas, x, y, r, fill_color);
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

void TermCanvas::line(Graphics::Point2D& pointi, Graphics::Point2D& pointf, Color value) {
    line(pointi.x, pointi.y, pointf.x, pointf.y, value);
}
void TermCanvas::line(Graphics::Point2D& pointi, Graphics::Point2D& pointf) {
    line(pointi.x, pointi.y, pointf.x, pointf.y);
}
void TermCanvas::line(Graphics::Point3D& pointi, Graphics::Point3D& pointf, Color value) {
    line(pointi.x, pointi.y, pointf.x, pointf.y, value);
}
void TermCanvas::line(Graphics::Point3D& pointi, Graphics::Point3D& pointf) {
    line(pointi.x, pointi.y, pointf.x, pointf.y);
}

void TermCanvas::drawPoint(Graphics::Point2D& point, Color color) {
    set(point.x, point.y, color);
}

void TermCanvas::drawPoint(Graphics::Point3D& point, Color color) {
    set(point.x, point.y, color);
}

void TermCanvas::drawPoint(Graphics::Point2D& point) {
    set(point.x, point.y);
}

void TermCanvas::drawPoint(Graphics::Point3D& point) {
    set(point.x, point.y);
}

void TermCanvas::drawPolygon(Graphics::Figure::Polygon2D& poly2d, Color color) {
    Graphics::Point2D head = poly2d[0];
    for (size_t i = 1; i < poly2d.size(); i++) {
        Graphics::Point2D p3d = poly2d[i];
        line(head, p3d, color);
        head = p3d;
    }
    line(head, poly2d[0], color);
}

void TermCanvas::drawPolygon(Graphics::Figure::Polygon3D& poly3d, Color color) {
    Graphics::Point3D head = poly3d[0];
    for (size_t i = 1; i < poly3d.size(); i++) {
        Graphics::Point3D p3d = poly3d[i];
        line(head, p3d, color);
        head = p3d;
    }
    line(head, poly3d[0], color);
}

void TermCanvas::drawPolygon(Graphics::Figure::Polygon2D& poly2d) {
    drawPolygon(poly2d, stroke_color);
}

void TermCanvas::drawPolygon(Graphics::Figure::Polygon3D& poly3d) {
    drawPolygon(poly3d, stroke_color);
}

void TermCanvas::drawShape(Graphics::Figure::Shape& shape, Color color) {
    for (Graphics::Figure::Polygon3D poly3d : shape ) {
        drawPolygon( poly3d, color );
    }
}
void TermCanvas::drawShape(Graphics::Figure::Shape& shape) {
    drawShape(shape, stroke_color);
}



// transform the current context
void TermCanvas::translate(uint32_t x, uint32_t y) {
    translate_x += x;
    translate_y += y;
}

void TermCanvas::moveTo(uint32_t x, uint32_t y) {
    translate_x = x;
    translate_y = y;
}

void TermCanvas::restore () {
    translate_x = 0;
    translate_y = 0;
}
