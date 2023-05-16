#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextEdit>
#include <QComboBox>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QAction>
#include <QPushButton>
#include <QPrinter>
#include <QPrintDialog>
#include <QInputDialog>
#include <QTextTable>
#include <QTextTableFormat>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextImageFormat>
#include <QColorDialog>

QIcon createColorIcon(const QColor &color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(color);

    return QIcon(pixmap);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    textEdit = new QTextEdit(this);

    layout = new QVBoxLayout(centralWidget);

    fontSizeComboBox = new QComboBox(this);
    fontSizeComboBox->setMaximumWidth(40);
    fontSizeComboBox->addItems({"9", "12", "15", "18","22","26","30","35","40","42"});
    connect(fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFontSize);
    layout->addWidget(fontSizeComboBox);

    textColorComboBox = new QComboBox(this);
    textColorComboBox->setMaximumWidth(100);
    textColorComboBox->addItem(createColorIcon(Qt::black), "Black");
    textColorComboBox->addItem(createColorIcon(Qt::red), "Red");
    textColorComboBox->addItem(createColorIcon(Qt::green), "Green");
    textColorComboBox->addItem(createColorIcon(Qt::blue), "Blue");
    connect(textColorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeTextColor);
    layout->addWidget(textColorComboBox);


    alignmentComboBox = new QComboBox(this);
    alignmentComboBox->setMaximumWidth(100);
    alignmentComboBox->addItem("Left", Qt::AlignLeft);
    alignmentComboBox->addItem("Center", Qt::AlignHCenter);
    alignmentComboBox->addItem("Right", Qt::AlignRight);


    connect(alignmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeAlignment);
    layout->addWidget(alignmentComboBox);

    fontComboBox = new QComboBox(this);
    fontComboBox->addItems(QFontDatabase::families());
    fontComboBox->setMaximumWidth(150);
    connect(fontComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFont);
    layout->addWidget(fontComboBox);

    boldComboBox = new QComboBox(this);
    boldComboBox->addItem("Bold");
    boldComboBox->setMaximumWidth(120);
    connect(boldComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleBold);

    italicComboBox = new QComboBox(this);
    italicComboBox->addItem("Italic");
    italicComboBox->setMaximumWidth(120);
    connect(italicComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleItalic);

    underlineComboBox = new QComboBox(this);
    underlineComboBox->addItem("Underline");
    underlineComboBox->setMaximumWidth(120);
    connect(underlineComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleUnderline);
    layout->addWidget(boldComboBox);
    layout->addWidget(italicComboBox);
    layout->addWidget(underlineComboBox);

    numberedListButton = new QPushButton("Numbered List", this);
    numberedListButton->setMaximumWidth(120);
    connect(numberedListButton, &QPushButton::clicked, this, &MainWindow::on_numberedListButton_clicked);
    layout->addWidget(numberedListButton);

    createTableButton = new QPushButton("Create table", this);
    createTableButton->setMaximumWidth(120);
    connect(createTableButton, &QPushButton::clicked, this, &MainWindow::createTable);
    layout->addWidget(createTableButton);

    insertImageButton = new QPushButton("Insert image", this);
    insertImageButton->setMaximumWidth(120);
    connect(insertImageButton, &QPushButton::clicked, this, &MainWindow::insertImage);
    layout->addWidget(insertImageButton);

    colorButton= new QPushButton("Background color", this);
    colorButton->setMaximumWidth(120);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::changeBackgroundColor);

    layout->addWidget(colorButton);

    spacingButton = new QPushButton("Set Line Spacing", this);
    spacingButton->setMaximumWidth(120);
    connect(spacingButton, &QPushButton::clicked, this, &MainWindow::setLineSpacing);
    layout->addWidget(spacingButton);

    layout->addWidget(textEdit);



}

void MainWindow::setLineSpacing()
{
    bool ok;
    int spacing = QInputDialog::getInt(this, tr("Térköz beállítása"), tr("Térköz (pixel):"), 0, 0, 100, 1, &ok);
        if (ok) {
            QTextBlockFormat blockFormat;
            blockFormat.setLineHeight(spacing, QTextBlockFormat::FixedHeight);

            QTextCursor cursor = textEdit->textCursor();
            cursor.select(QTextCursor::Document);
            cursor.mergeBlockFormat(blockFormat);
            cursor.clearSelection();

            // Opcionális: alapértelmezett betűtípus beállítása
            QFont defaultFont("Arial", 12);
            textEdit->setFont(defaultFont);
        }
}

void MainWindow::changeBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Válasszon háttérszínt");
    if (color.isValid()) {
        QString styleSheet = QString("background-color: %1").arg(color.name());
        textEdit->setStyleSheet(styleSheet);
    }
}

void MainWindow::toggleBold()
{
    QTextCharFormat format;
    format.setFontWeight(textEdit->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    textEdit->setFontWeight(format.fontWeight());
}


// Dőlt dekorátor
void MainWindow::toggleItalic()
{
    QTextCharFormat format;
    format.setFontItalic(!textEdit->fontItalic());
    textEdit->setFontItalic(format.fontItalic());
}

// Aláhúzott dekorátor
void MainWindow::toggleUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(!textEdit->fontUnderline());
    textEdit->setFontUnderline(format.fontUnderline());
}

void MainWindow::changeFont(int index)
{
    QString fontFamily = fontComboBox->itemText(index);

    textEdit->setFontFamily(fontFamily);
}

void MainWindow::changeAlignment(int index)
{
    Qt::AlignmentFlag alignment = alignmentComboBox->itemData(index).value<Qt::AlignmentFlag>();

     textEdit->setAlignment(alignment);
}

void MainWindow::changeFontSize(int index)
{
   QString selectedSize = fontSizeComboBox->currentText();
   int fontSize = selectedSize.toInt();

    if (fontSize >= 9 && fontSize <= 42) {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format;
        format.setFontPointSize(fontSize);
        cursor.mergeCharFormat(format);
        textEdit->mergeCurrentCharFormat(format);
    }
}

void MainWindow::changeTextColor(int index)
{
    QString selectedColor = textColorComboBox->currentText();
    QColor color;

    if (selectedColor == "Black")
        color = Qt::black;
    else if (selectedColor == "Red")
        color = Qt::red;
    else if (selectedColor == "Green")
        color = Qt::green;
    else if (selectedColor == "Blue")
        color = Qt::blue;

    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format;
    format.setForeground(color);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createNumberedList()
{
    QTextDocument* document = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();

    // Ellenőrizzük, hogy van-e kijelölt szöveg
    if (!cursor.hasSelection())
        return;

    // Lekérdezzük a kijelölt szöveg kezdő és végpozícióját
    int selectionStart = cursor.selectionStart();
    int selectionEnd = cursor.selectionEnd();

    // Mozgatjuk a kurzort a kijelölés kezdő pozíciójára
    cursor.setPosition(selectionStart);

    // Végigmegyünk a sorokon a kijelölt szövegrészben és számozzuk őket
    int lineNumber = 1;
    while (cursor.position() < selectionEnd) {
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.insertText(QString::number(lineNumber) + ". ");
        cursor.movePosition(QTextCursor::NextBlock);
        lineNumber++;
    }

    // Frissítjük a textEdit tartalmát
    textEdit->setDocument(document);
}

void MainWindow::on_numberedListButton_clicked()
{
    createNumberedList();
}

void MainWindow::on_actionExit_triggered()
{
    this->close(); // Az aktuális ablak bezárása
    QApplication::quit(); // Az alkalmazás bezárása
}


void MainWindow::on_actionOpen_triggered()
{   if(fontSizeComboBox){

        fontSizeComboBox->setMaximumWidth(40);

        layout->addWidget(fontSizeComboBox);

        layout->addWidget(textColorComboBox);

        alignmentComboBox->setMaximumWidth(100);

        layout->addWidget(alignmentComboBox);

        fontComboBox->setMaximumWidth(150);

        layout->addWidget(fontComboBox);

        boldComboBox->setMaximumWidth(120);

        italicComboBox->setMaximumWidth(120);

        underlineComboBox->setMaximumWidth(120);

        layout->addWidget(boldComboBox);
        layout->addWidget(italicComboBox);
        layout->addWidget(underlineComboBox);

        numberedListButton->setMaximumWidth(120);

        createTableButton->setMaximumWidth(120);
        layout->addWidget(createTableButton);

        insertImageButton->setMaximumWidth(120);

        layout->addWidget(insertImageButton);

        colorButton->setMaximumWidth(120);

        layout->addWidget(colorButton);

        spacingButton->setMaximumWidth(120);
        layout->addWidget(spacingButton);

        layout->addWidget(numberedListButton);
    }
    else{
        fontSizeComboBox = new QComboBox(this);
        fontSizeComboBox->setMaximumWidth(40);
        fontSizeComboBox->addItems({"9", "12", "15", "18","22","26","30","35","40","42"});
        connect(fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFontSize);
        layout->addWidget(fontSizeComboBox);

        textColorComboBox = new QComboBox(this);
        textColorComboBox->setMaximumWidth(100);
        textColorComboBox->addItem(createColorIcon(Qt::black), "Black");
        textColorComboBox->addItem(createColorIcon(Qt::red), "Red");
        textColorComboBox->addItem(createColorIcon(Qt::green), "Green");
        textColorComboBox->addItem(createColorIcon(Qt::blue), "Blue");
        connect(textColorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeTextColor);

        layout->addWidget(textColorComboBox);

        alignmentComboBox = new QComboBox(this);
        alignmentComboBox->setMaximumWidth(100);
        alignmentComboBox->addItem("Left", Qt::AlignLeft);
        alignmentComboBox->addItem("Center", Qt::AlignHCenter);
        alignmentComboBox->addItem("Right", Qt::AlignRight);

        connect(alignmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeAlignment);
        layout->addWidget(alignmentComboBox);


        fontComboBox = new QComboBox(this);
        fontComboBox->addItems(QFontDatabase::families());
        fontComboBox->setMaximumWidth(150);
        connect(fontComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFont);
        layout->addWidget(fontComboBox);

        boldComboBox = new QComboBox(this);
        boldComboBox->addItem("Bold");
        boldComboBox->setMaximumWidth(120);
        connect(boldComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleBold);

        italicComboBox = new QComboBox(this);
        italicComboBox->addItem("Italic");
        italicComboBox->setMaximumWidth(120);
        connect(italicComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleItalic);

        underlineComboBox = new QComboBox(this);
        underlineComboBox->addItem("Underline");
        underlineComboBox->setMaximumWidth(120);
        connect(underlineComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleUnderline);
        layout->addWidget(boldComboBox);
        layout->addWidget(italicComboBox);
        layout->addWidget(underlineComboBox);

        numberedListButton = new QPushButton("Numbered List", this);
        connect(numberedListButton, &QPushButton::clicked, this, &MainWindow::on_numberedListButton_clicked);
        numberedListButton->setMaximumWidth(120);

        createTableButton = new QPushButton("Táblázat létrehozása", this);
        createTableButton->setMaximumWidth(120);
        connect(createTableButton, &QPushButton::clicked, this, &MainWindow::createTable);
        layout->addWidget(createTableButton);

        insertImageButton = new QPushButton("Insert image", this);
        insertImageButton->setMaximumWidth(120);
        connect(insertImageButton, &QPushButton::clicked, this, &MainWindow::insertImage);

        layout->addWidget(insertImageButton);

        colorButton= new QPushButton("Background color", this);
        colorButton->setMaximumWidth(120);
        connect(colorButton, &QPushButton::clicked, this, &MainWindow::changeBackgroundColor);

        layout->addWidget(colorButton);

        spacingButton = new QPushButton("Set Line Spacing", this);
        spacingButton->setMaximumWidth(120);
        connect(spacingButton, &QPushButton::clicked, this, &MainWindow::setLineSpacing);
        layout->addWidget(spacingButton);

        layout->addWidget(numberedListButton);

    }

    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text files (*.txt);;RTF files (*.rtf)");
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            QString content = stream.readAll();
            QString fileExtension = QFileInfo(filePath).suffix();
            file.close();
            if(fileExtension == "rtf"){
            // Fájl tartalmának megjelenítése a textEdit widgetben
            if (textEdit) {
                // Ha a textEdit widget már létezik, csak frissítsd a tartalmát
                textEdit->setHtml(content);
            } else {
                // Ha a textEdit widget még nem létezik, hozd létre és állítsd be a központi widgetként
                textEdit = new QTextEdit(this);
                layout -> addWidget(textEdit);
                textEdit->setHtml(content);
            }
            }
            else {
            if (textEdit) {
                // Ha a textEdit widget már létezik, csak frissítsd a tartalmát
                textEdit->setPlainText(content);
            }
            else {
                // Ha a textEdit widget még nem létezik, hozd létre és állítsd be a központi widgetként
                textEdit = new QTextEdit(this);
                layout -> addWidget(textEdit);
                textEdit->setPlainText(content);
            }
            }
        }
    }


}


void MainWindow::on_actionClose_triggered()
{
    if (textEdit) {
        delete textEdit;
        textEdit = nullptr;
        delete fontSizeComboBox;
        fontSizeComboBox = nullptr;
        delete textColorComboBox;
        textColorComboBox = nullptr;
        delete alignmentComboBox;
        alignmentComboBox = nullptr;
        delete fontComboBox;
        fontComboBox = nullptr;
        delete boldComboBox;
        delete italicComboBox;
        delete underlineComboBox;
        boldComboBox = nullptr;
        italicComboBox = nullptr;
        underlineComboBox = nullptr;
        delete numberedListButton;
        numberedListButton = nullptr;
        delete createTableButton;
        createTableButton = nullptr;
        delete insertImageButton;
        insertImageButton = nullptr;
        delete  colorButton;
        colorButton = nullptr;
        delete spacingButton;
        spacingButton = nullptr;
    }
}


void MainWindow::on_actionSave_triggered()
{
    if (textEdit != nullptr && !textEdit->toPlainText().isEmpty()) {
        QString filePath = QFileDialog::getSaveFileName(this, "Save as", "", "RTF files (*.rtf);;Text files (*.txt)");

        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                QString fileExtension = QFileInfo(filePath).suffix();

                if (fileExtension == "rtf") {
                    out << textEdit->toHtml();
                }
                else{
                   out << textEdit->toPlainText();
                }

                file.close();
            }
        }
    }
}

void MainWindow::createTable()
{
    bool ok;
    int rows = QInputDialog::getInt(this, tr("Táblázat létrehozása"), tr("Sorok száma:"), 3, 1, 100, 1, &ok);
    if (!ok) return;

    int columns = QInputDialog::getInt(this, tr("Táblázat létrehozása"), tr("Oszlopok száma:"), 3, 1, 100, 1, &ok);
    if (!ok) return;

    QTextCursor cursor = textEdit->textCursor();

    QTextTableFormat tableFormat;
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
    tableFormat.setCellPadding(5);
    tableFormat.setCellSpacing(0);
    QTextTable *table = cursor.insertTable(rows, columns, tableFormat);

    // Beállítjuk a cellák tartalmát
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            QTextTableCell cell = table->cellAt(row, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            QString text = QInputDialog::getText(this, tr("Cella tartalma"), tr("Írja be a(z) %1 sor %2 oszlop tartalmát:").arg(row + 1).arg(col + 1));
            cellCursor.insertText(text);
        }
    }
}

void MainWindow::insertImage()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Kép kiválasztása"), "", tr("Képfájlok (*.png *.jpg *.bmp)"));
    if (imagePath.isEmpty())
        return;

    QImage image(imagePath);
    if (image.isNull())
    {
        QMessageBox::warning(this, tr("Hiba"), tr("Nem sikerült betölteni a képet."));
        return;
    }
    QSize imageSize(120, 120);
    image = image.scaled(imageSize, Qt::KeepAspectRatio);
    QTextCursor cursor = textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setName(imagePath);
    imageFormat.setWidth(image.width());
    imageFormat.setHeight(image.height());
    cursor.insertImage(imageFormat);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument* document = textEdit->document();
        document->print(&printer);
    }
}

