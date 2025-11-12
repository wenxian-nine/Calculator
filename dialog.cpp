#include "dialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <stack>
#include <string>
#include <cctype>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QGridLayout* editLayout = new QGridLayout;
    QGridLayout* buttonLayout = new QGridLayout;
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(buttonLayout);

    Edit = new QLineEdit;
    editLayout->addWidget(Edit);
    Edit->setAlignment(Qt::AlignRight);

    QPushButton* deleteButton = new QPushButton("<-");
    QPushButton* divisionButton = new QPushButton("/");
    QPushButton* multiplicationButton = new QPushButton("*");
    QPushButton* subtractionButton = new QPushButton("-");

    QPushButton* sevenButton = new QPushButton("7");
    QPushButton* eightButton = new QPushButton("8");
    QPushButton* nineButton = new QPushButton("9");
    QPushButton* additionButton = new QPushButton("+");

    QPushButton* fourButton = new QPushButton("4");
    QPushButton* fiveButton = new QPushButton("5");
    QPushButton* sixButton = new QPushButton("6");

    QPushButton* oneButton = new QPushButton("1");
    QPushButton* twoButton = new QPushButton("2");
    QPushButton* threeButton = new QPushButton("3");
    QPushButton* enterButton = new QPushButton("Enter");

    QPushButton* zeroButton = new QPushButton("0");
    QPushButton* pointButton = new QPushButton(".");

    buttonLayout->addWidget(deleteButton,0,0);
    buttonLayout->addWidget(divisionButton,0,1);
    buttonLayout->addWidget(multiplicationButton,0,2);
    buttonLayout->addWidget(subtractionButton,0,3);

    buttonLayout->addWidget(sevenButton,1,0);
    buttonLayout->addWidget(eightButton,1,1);
    buttonLayout->addWidget(nineButton,1,2);
    buttonLayout->addWidget(additionButton,1,3,2,1);
    additionButton->setFixedSize(80, 60);

    buttonLayout->addWidget(fourButton,2,0);
    buttonLayout->addWidget(fiveButton,2,1);
    buttonLayout->addWidget(sixButton,2,2);

    buttonLayout->addWidget(oneButton,3,0);
    buttonLayout->addWidget(twoButton,3,1);
    buttonLayout->addWidget(threeButton,3,2);
    buttonLayout->addWidget(enterButton,3,3,2,1);
    enterButton->setFixedSize(80, 60);

    buttonLayout->addWidget(zeroButton,4,0,1,2);
    buttonLayout->addWidget(pointButton,4,2);

    // 數字與符號
    connect(zeroButton, &QPushButton::clicked, this, [=]() {Edit->insert("0");});
    connect(oneButton, &QPushButton::clicked, this, [=]() {Edit->insert("1");});
    connect(twoButton, &QPushButton::clicked, this, [=]() {Edit->insert("2");});
    connect(threeButton, &QPushButton::clicked, this, [=]() {Edit->insert("3");});
    connect(fourButton, &QPushButton::clicked, this, [=]() {Edit->insert("4");});
    connect(fiveButton, &QPushButton::clicked, this, [=]() {Edit->insert("5");});
    connect(sixButton, &QPushButton::clicked, this, [=]() {Edit->insert("6");});
    connect(sevenButton, &QPushButton::clicked, this, [=]() {Edit->insert("7");});
    connect(eightButton, &QPushButton::clicked, this, [=]() {Edit->insert("8");});
    connect(nineButton, &QPushButton::clicked, this, [=]() {Edit->insert("9");});
    connect(pointButton, &QPushButton::clicked, this, [=]() {Edit->insert(".");});

    connect(additionButton, &QPushButton::clicked, this, [=]() {Edit->insert("+");});
    connect(subtractionButton, &QPushButton::clicked, this, [=]() {Edit->insert("-");});
    connect(multiplicationButton, &QPushButton::clicked, this, [=]() {Edit->insert("*");});
    connect(divisionButton, &QPushButton::clicked, this, [=]() {Edit->insert("/");});

    connect(deleteButton, &QPushButton::clicked, this, &Dialog::deletebtn);
    connect(enterButton, &QPushButton::clicked, this, &Dialog::enterbtn);
}

void Dialog::deletebtn()
{
    QString text = Edit->text();
    if (!text.isEmpty()) {
        text.chop(1);
        Edit->setText(text);
    }
}

void Dialog::enterbtn()
{
    QString expression = Edit->text();
    if (expression.isEmpty())
        return;

    std::string expr = expression.toStdString();

    auto precedence = [](char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    auto applyOp = [](double a, double b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return 0.0;
        }
    };

    std::stack<double> values;
    std::stack<char> ops;
    int i = 0;

    while (i < (int)expr.size()) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.') {
            std::string val;
            while (i < (int)expr.size() && (isdigit(expr[i]) || expr[i] == '.'))
                val += expr[i++];
            values.push(std::stod(val));
        } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(expr[i]);
            i++;
        } else {
            Edit->setText("Error");
            return;
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    double result = values.top();
    Edit->setText(QString::number(result));
}

Dialog::~Dialog() {}
