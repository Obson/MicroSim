//
//  government.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Government *Government::_instance = nullptr;

Government *Government::Instance()
{
    if (_instance == nullptr) {
        _instance = new Government();
    }
    return _instance;
}

void Government::init()
{
    exp = 0;
    rec = 0;
    ben = 0;
    
    // This must be the first Firm created, as this ensures that government
    // money is always spent at the start of the period. However, we defer it
    // t here because if it's in the constructure you get a recursive
    // declaration: Givernment instantiates Firm, which instantiates
    // Government).
    gov_firm = reg->createFirm();
    
}

Government::Government()
{
    reg = Register::Instance();
}

// TO DO NEXT. OR AT LEAST VERY SOON
// ---------------------------------
// This method creates and registers a new independent firm with, at present,
// no visible means of support. To fix this we will need to provide a means
// of accessing government funds. Conventionally this is achieved via banks,
// so what we probably need to do is implement the Bank class and set the
// new firm up with a 'line of credit'. I think this means that the firm can
// keep borrowing more money as long as it keeps up the repayments on the
// existing loan and pays the interest due. I suspect this is inherently
// unstable and is only sustainable as long as the firm continues to grow.
// Eventually the market becomes saturated and the firm dies -- or possibly
// it just sucks in business from other less successful firms in the same
// field, and they die.

int Government::getExpenditure()
{
    return exp;
}

int Government::getBenefitsPaid()
{
    return ben;
}

int Government::getReceipts()
{
    return rec;
}


void Government::trigger(int period)
{
    //std::cout << "\nGovernment::trigger(" << period << ")\n";
    
    init();
    reg->init();
    
    //
    // Government grants (incl support of gov-owned businesses)
    //
    int amt = settings->getGovExpRate();
    gov_firm->grant(amt);
    balance -= amt;
    exp += amt;
    
    //
    // Benefits
    //
    
    reg->trigger(period);
    
    ben += reg->payWorkers((settings->getStdWage() * settings->getUBR()) / 100,
                           0,                   // no max amount
                           nullptr,             // no source (i.e. Government)
                           Register::benefits,  // reason
                           period
                           );
}

//
// This function overrides Account::transferTo to allow a negative balance.
//
void Government::transferTo(Account *recipient, int amount, Account *creditor)
{
    // We adopt the convention that receipts from the government are not
    // taxable. This is probably a rather murky area, given that the
    // mechanisms by which government injects money into the economy are
    // unclear and seem to involve financing banks to make more loans.
    // In the case of the NHS, nationalised industries, the civil service
    // and the 'armed forces' the mechanism is probably more direct.
    // Anyway, to go into this would be a distraction so we'll simply
    // treat it as untaxable payment for services.
    recipient->credit(amount, this);
    balance -= amount;
}

// All credits to the Government are (at present) regarded as tax. This
// means that while they offset the deficit we need to keep a separate
// record as well. However we don't distinguish between income tax, sales
// tax, and 'pre-tax deductions'. These are all accounted for elsewhere.
// Obviously, the government doesn't pay tax.
void Government::credit(int amount, Account *creditor)
{
    Account::credit(amount);
    rec += amount;
}



