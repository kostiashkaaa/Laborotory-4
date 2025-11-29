
#ifndef ADDTARIFFDIALOG_H
#define ADDTARIFFDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include "Tariff.h"

class AddTariffDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit AddTariffDialog(QWidget *parent = nullptr);
    
    AddTariffDialog(QWidget *parent, const Tariff& tariff);
    
    ~AddTariffDialog();
    
    Tariff getTariff() const;
    
private slots:
    void onAccept();
    void onCancel();
    
private:
    QLineEdit *cityEdit;
    QDoubleSpinBox *priceSpinBox;
    QDoubleSpinBox *connectionFeeSpinBox;
    
    bool isEditMode;
    
    void setupUI();
    void validateAndAccept();
};

#endif
