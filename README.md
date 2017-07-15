# MicroSim

**MicroSim** is a terminal-based micro-economic modelling program designed to complement SimX (which is a *macro*-economic modelling system). Unlike SimX, which provided its own graphics, it outputs its results in the form of a CSV file which can be read into a spreadsheet for further analysis.


## Components of a Model and Their Interactions ##

A model contains the following components:

* Workers
* Firms
* A Government (later versions may allow more thn one Government)
* Banks

These components interact in fairly simple ways in response to 

* the passing of time
* the flow of money

Time is measured in *periods* (AKA *cycles* or *iterations*). These do not correspond in any precise or consistent way to time in the real world. Thus, for example, in the model all firms pay their employees once per period, while in the real world wages and salaries may be paid weekly or monthly, or sometimes four weekly or at varying intervals. This is one of the limitations of the modelling system.

Money in MicroSim is curently dimensionless and may be thought of as arbitrary *currency units* or CUs. This will probably change when we add foreign exchange, later.

Every component is *triggered* once per period, and when triggered it will do something depending on its circumstances at the time. A firm, for example, may hire or fire employees, and a worker may make some purchases. The parameters of the model will determine more precisely how they respond. 

## Parameters of a Model ##

The parameters that define the model must be supplied in a text file, which by default is named ```basic.mod```. To use a different file enter its name as the first argument when invoking the program. For example:

~~~
microsim myfile.mod
~~~

The number of iterations can be given as the second argument on the command line, as in

~~~
microsim basic.mod 1000
~~~

or included in the model definition file. If omitted altogether, 10 will be assumed.

### Structure of a Model Definition File ###

(At present) a typical model definition file looks something like this:

~~~
Basic Model + Investment Rate Reduced To 50%

Population size 10000
Employment rate 100
Standard wage 500
Consumption rate 100
Deductions rate 0
Income tax rate 10
Sales tax rate 0
Business creation rate 0
Reserve rate 0
Investment rate 20
Unemployment benefit rate 0

The Basic Model has only one employer (the government), and businesses
invest all their funds in recruiting new employees rather than in paying
bonuses or dividends.
~~~
