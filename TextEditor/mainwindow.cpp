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
    layout->addWidget(textEdit);



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


void MainWindow::on_actionExit_triggered()
{
    this->close(); // Az aktuális ablak bezárása
    QApplication::quit(); // Az alkalmazás bezárása
}


void MainWindow::on_actionOpen_triggered()
{   if(fontSizeComboBox){

        fontSizeComboBox->setMaximumWidth(40);
        connect(fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFontSize);
        layout->addWidget(fontSizeComboBox);

        layout->addWidget(textColorComboBox);

        alignmentComboBox->setMaximumWidth(100);
        connect(alignmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeAlignment);
        layout->addWidget(alignmentComboBox);

        fontComboBox->setMaximumWidth(150);
        connect(fontComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeFont);
        layout->addWidget(fontComboBox);



        boldComboBox->setMaximumWidth(120);
        connect(boldComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleBold);



        italicComboBox->setMaximumWidth(120);
        connect(italicComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleItalic);


        underlineComboBox->setMaximumWidth(120);
        connect(underlineComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::toggleUnderline);

        layout->addWidget(boldComboBox);
        layout->addWidget(italicComboBox);
        layout->addWidget(underlineComboBox);
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

