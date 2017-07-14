# MicroSim
Micro-economic modelling system

MicroSim is a terminal-based micro-economic modelling program designed to complement SimX (which works at the macro level). Unlike SimX, which provided its own graphics, it outputs its results as a CSV file which is intended to be read into a spreadsheet for further analysis.

The parameters that define the model must be supplied in a text file, which by default is named basic.mod. To use a different file enter its name as the first argument when invoking the program. For example:
```
microsim myfile.mod
```
The number of iterations can be given as the second argument on the command line, as in
```
microsim basic.mod 1000
```
or included in the model definition file. If omitted altogether, 10 will be assumed.

## Structure of a Model Definition File

(At present) a typical model definition file looks something like this:
```
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

The Basic Model has only one employer (the government, and businesses
invest all their funds in recruiting new employees rather than in paying
bonuses or dividends.
```
