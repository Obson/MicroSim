# MicroSim

**MicroSim** is a terminal-based micro-economic modelling program designed to complement SimX (which is a *macro*-economic modelling system). Unlike SimX, which provides its own graphics, microsim outputs its results in the form of a CSV file which can be read into a spreadsheet for further analysis.


## Components of a Model and Their Interactions ##

A model contains the following components:

* Workers
* Firms
* A Government (later versions may allow more thn one Government)
* Banks

These components interact in fairly simple ways in response to 

* the passing of time
* the flow of money

**Time** is measured in *periods* (AKA *cycles* or *iterations*). These do not correspond in any precise or consistent way to time in the real world. Thus, for example, in the model all firms pay their employees once per period, while in the real world wages and salaries may be paid weekly or monthly, or sometimes four weekly or at varying intervals. This is one of the limitations of the modelling system.

**Money** in MicroSim is curently dimensionless and may be thought of as arbitrary *currency units* or CUs. This will probably change when we add foreign exchange, later.

Every component is *triggered* once per period, and when triggered it will do something depending on its circumstances at the time. A firm, for example, may hire or fire employees, and a worker may make some purchases. The parameters of the model will determine more precisely how they respond. 

## How it Works ##

The model is entirely 'driven by' money. And as in the real world money emanates in the first instance from the government. There are a number of ways money may be disbursed, for example:

1. To businesses
	- as a direct payment or grant
	- by purchasing goods or services
	- via banks as loans (see e.g. Mosler for a description of the of how governments finance loans issued by banks)
	- as interest payments ongovernment bonds.
2. To individuals as benefits (e.g. unemployment benefit)

At this stage we will ignore the payment of interest on government bonds as this is really an irrelevance. (*We will also ignore bank loans for the time being as banking isn't yet implemented.*) 

### The clock ticks... ###

The first thing that happens each time the clock ticks (i.e. at the beginning of each period) is that the government spends a certain amount of money 'into the economy'. One of the ways it does this in the real world is by supporting nationalised indistries, the armed forces, the NHS, the civil service, parliament and the royal family (and no doubt a number of other things I don't know about or have overlooked). Another is by paying 'benefits' (such as unemployment benefit) to individuals.

The question then is how much the government should pay.

### Government expenditure ###

The amount the government needs to spend (ignoring benefits) is determined by the size of the economy, the wage rate, the tax rate, and the rate of investment in employment. In the real world these quantities, with the exception of the tax rate, are quite hard to determine. For MicroSim the situation is much simpler&mdash;they are all parameters given in the model definition. We give, for example, an actual population size and a target rate of employment. By multiplying these together MicroSim obtains the size of the economically active population.

...  


## Parameters of a Model ##

The parameters that define a model must be supplied in a text file, which by default is named ```basic.mod```. To use the parameters in the default model just call the program without any arguments:

~~~
microsim
~~~

The model definition file must be in the working directory.

To use a different model definition file enter its name as the first argument when invoking the program. For example:

~~~
microsim myfile.mod
~~~

The number of iterations defaults to 10 if it is not given as a program argument or in the model definition file. Alternatively it can be given as the second argument on the command line, as in

~~~
microsim basic.mod 1000
~~~

When the number of iterations is given on the command line this value takes precedence over the value (if any) in the definition file.
 
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
