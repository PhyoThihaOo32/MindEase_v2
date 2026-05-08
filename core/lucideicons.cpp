#include "core/lucideicons.h" // Custom Lucide-style icon helpers

#include <QPainter> // Used for drawing icons
#include <QPainterPath> // Used for custom vector shapes
#include <QPen> // Defines line style/color/thickness
#include <QPolygonF> // Polygon drawing support
#include <QRegularExpression> // Text cleanup and replacements
#include <QtGlobal> // Qt global utilities

#include <cmath> // Math functions

namespace {

// Checks if a token contains a keyword or emoji
bool has(const QString &token, const QString &needle) {
    return token.contains(needle);
}

// Draws open book icon
void drawBook(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 5, 7, 14), 1.5, 1.5);
    p.drawRoundedRect(QRectF(13, 5, 7, 14), 1.5, 1.5);
    p.drawLine(QPointF(11, 6), QPointF(11, 20));
    p.drawLine(QPointF(13, 6), QPointF(13, 20));
}

// Draws dollar sign icon
void drawDollar(QPainter &p) {
    QFont f = p.font();
    f.setPixelSize(17);
    f.setBold(true);
    p.setFont(f);
    p.drawLine(QPointF(12, 3), QPointF(12, 21));
    p.drawText(QRectF(4, 2, 16, 20), Qt::AlignCenter, "S");
}

// Draws globe icon
void drawGlobe(QPainter &p) {
    p.drawEllipse(QRectF(4, 4, 16, 16));
    p.drawLine(QPointF(4, 12), QPointF(20, 12));
    p.drawArc(QRectF(7, 4, 10, 16), 90 * 16, 180 * 16);
    p.drawArc(QRectF(7, 4, 10, 16), -90 * 16, 180 * 16);
}

// Draws chat/message bubble
void drawMessage(QPainter &p) {
    QPainterPath path;
    path.addRoundedRect(QRectF(4, 5, 16, 12), 4, 4);
    p.drawPath(path);
    p.drawLine(QPointF(9, 17), QPointF(6, 21));
    p.drawLine(QPointF(9, 17), QPointF(12, 17));
}

// Draws hospital icon
void drawHospital(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 4, 14, 17), 1.5, 1.5);
    p.drawLine(QPointF(12, 8), QPointF(12, 15));
    p.drawLine(QPointF(8.5, 11.5), QPointF(15.5, 11.5));
    p.drawLine(QPointF(8, 21), QPointF(8, 17));
    p.drawLine(QPointF(16, 21), QPointF(16, 17));
}

// Draws briefcase icon
void drawBriefcase(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 8, 16, 11), 2, 2);
    p.drawRoundedRect(QRectF(9, 5, 6, 3), 1, 1);
    p.drawLine(QPointF(4, 12), QPointF(20, 12));
}

// Draws handshake icon
void drawHandshake(QPainter &p) {
    p.drawLine(QPointF(4, 12), QPointF(8, 8));
    p.drawLine(QPointF(20, 12), QPointF(16, 8));
    p.drawLine(QPointF(8, 8), QPointF(11, 11));
    p.drawLine(QPointF(16, 8), QPointF(13, 11));
    p.drawLine(QPointF(9, 13), QPointF(12, 16));
    p.drawLine(QPointF(12, 16), QPointF(15, 13));
    p.drawLine(QPointF(7, 14), QPointF(11, 18));
    p.drawLine(QPointF(17, 14), QPointF(13, 18));
}

// Draws pencil/edit icon
void drawPencil(QPainter &p) {
    p.drawLine(QPointF(5, 19), QPointF(16.5, 7.5));
    p.drawLine(QPointF(14, 5), QPointF(19, 10));
    p.drawLine(QPointF(16.5, 7.5), QPointF(14, 5));
    p.drawLine(QPointF(19, 10), QPointF(7.5, 21.5));
    p.drawLine(QPointF(5, 19), QPointF(7.5, 21.5));
}

// Draws brain icon
void drawBrain(QPainter &p) {
    p.drawArc(QRectF(4, 6, 8, 8), 40 * 16, 230 * 16);
    p.drawArc(QRectF(12, 6, 8, 8), -90 * 16, 230 * 16);
    p.drawArc(QRectF(6, 11, 7, 8), 110 * 16, 180 * 16);
    p.drawArc(QRectF(11, 11, 7, 8), -110 * 16, 180 * 16);
    p.drawLine(QPointF(12, 5), QPointF(12, 20));
}

// Draws calendar icon
void drawCalendar(QPainter &p) {
    p.drawRoundedRect(QRectF(4, 5, 16, 15), 2, 2);
    p.drawLine(QPointF(4, 9), QPointF(20, 9));
    p.drawLine(QPointF(8, 3), QPointF(8, 7));
    p.drawLine(QPointF(16, 3), QPointF(16, 7));
}

// Draws puzzle icon
void drawPuzzle(QPainter &p) {
    QPainterPath path;
    path.moveTo(6, 6);
    path.lineTo(10, 6);
    path.cubicTo(10, 3.5, 14, 3.5, 14, 6);
    path.lineTo(18, 6);
    path.lineTo(18, 11);
    path.cubicTo(20.5, 11, 20.5, 15, 18, 15);
    path.lineTo(18, 20);
    path.lineTo(6, 20);
    path.lineTo(6, 15);
    path.cubicTo(8.5, 15, 8.5, 11, 6, 11);
    path.closeSubpath();
    p.drawPath(path);
}

// Draws coffee cup icon
void drawCoffee(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 9, 11, 8), 2, 2);
    p.drawArc(QRectF(14, 10, 6, 5), -80 * 16, 260 * 16);
    p.drawLine(QPointF(5, 20), QPointF(18, 20));
    p.drawLine(QPointF(8, 5), QPointF(8, 7));
    p.drawLine(QPointF(12, 4), QPointF(12, 7));
}

// Draws school building icon
void drawSchool(QPainter &p) {
    p.drawPolyline(QPolygonF() << QPointF(4, 10) << QPointF(12, 5) << QPointF(20, 10));
    p.drawRect(QRectF(6, 10, 12, 10));
    p.drawLine(QPointF(9, 20), QPointF(9, 14));
    p.drawLine(QPointF(15, 20), QPointF(15, 14));
    p.drawLine(QPointF(4, 20), QPointF(20, 20));
}

// Draws search icon
void drawSearch(QPainter &p) {
    p.drawEllipse(QRectF(5, 5, 10, 10));
    p.drawLine(QPointF(14, 14), QPointF(20, 20));
}

// Default fallback icon
void drawDefault(QPainter &p) {
    p.drawRoundedRect(QRectF(5, 5, 14, 14), 4, 4);
    p.drawLine(QPointF(9, 12), QPointF(15, 12));
    p.drawLine(QPointF(12, 9), QPointF(12, 15));
}

} // namespace

// Converts emoji/token into icon name
QString lucideIconNameForToken(const QString &token) {
    const QString t = token.trimmed().toLower();

    if (t == "search" || has(token, "🔍") || has(token, "⌕")) return "search";
    if (has(token, "📚") || has(token, "📖") || has(token, "📘")) return "book-open";
    if (has(token, "💰")) return "dollar-sign";
    if (has(token, "🌐")) return "globe-2";
    if (has(token, "💬")) return "message-circle";
    if (has(token, "🏥")) return "hospital";
    if (has(token, "💼")) return "briefcase";

    return t.isEmpty() ? "circle" : t;
}

// Creates pixmap icon using custom painter drawing
QPixmap lucidePixmap(const QString &tokenOrName, const QColor &stroke, int size) {
    const int safeSize = qMax(12, size); // Prevents tiny icons

    QPixmap pixmap(safeSize, safeSize); // Creates transparent image
    pixmap.fill(Qt::transparent);

    QPainter p(&pixmap); // Painter object
    p.setRenderHint(QPainter::Antialiasing, true); // Smooth edges
    p.scale(safeSize / 24.0, safeSize / 24.0); // Scales icon size

    QPen pen(stroke, 1.9, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    const QString icon = lucideIconNameForToken(tokenOrName);

    if (icon == "book-open") drawBook(p);
    else if (icon == "dollar-sign") drawDollar(p);
    else if (icon == "globe-2") drawGlobe(p);
    else if (icon == "message-circle") drawMessage(p);
    else if (icon == "hospital") drawHospital(p);
    else if (icon == "briefcase") drawBriefcase(p);
    else if (icon == "handshake") drawHandshake(p);
    else if (icon == "pencil") drawPencil(p);
    else if (icon == "brain") drawBrain(p);
    else if (icon == "calendar") drawCalendar(p);
    else if (icon == "puzzle") drawPuzzle(p);
    else if (icon == "coffee") drawCoffee(p);
    else if (icon == "school") drawSchool(p);
    else if (icon == "search") drawSearch(p);
    else {
        drawDefault(p); // Fallback icon
    }

    return pixmap; // Returns generated icon image
}

// Converts pixmap into Qt icon object
QIcon lucideIcon(const QString &tokenOrName, const QColor &stroke, int size) {
    return QIcon(lucidePixmap(tokenOrName, stroke, size));
}

// Sets generated icon directly onto QLabel
void setLucideIcon(QLabel *label, const QString &tokenOrName, const QColor &stroke, int size) {
    if (!label) return;

    label->setText(QString{}); // Removes existing text
    label->setPixmap(lucidePixmap(tokenOrName, stroke, size)); // Sets icon image
    label->setAlignment(Qt::AlignCenter); // Centers icon
}

// Cleans emoji-heavy text into readable labels
QString lucideCleanText(QString text) {
    const QList<QPair<QString, QString>> replacements = {
        { "📍", "Location:" },
        { "📞", "Phone:" },
        { "☎️", "Phone:" },
        { "📧", "Email:" },
        { "🕐", "Hours:" },
        { "👤", "Contact:" },
        { "✓", "" },
        { "🔍", "" }
    };

    for (const auto &replacement : replacements) {
        text.replace(replacement.first, replacement.second); // Replaces emojis with labels
    }

    text.replace(QRegularExpression("[\\t ]+"), " "); // Removes extra spaces
    text.replace(QRegularExpression(" *\\n *"), "\n"); // Cleans line spacing

    return text.trimmed().replace(" | ", "  |  "); // Final formatting cleanup
}