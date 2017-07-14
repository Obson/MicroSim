# MicroSim
Micro-economic modelling system

MicroSim is a terminal-based micro-economic modelling program designed to complement SimX (which works at the macro level). Unlike SimX, which provided its own graphics, it outputs its results as a CSV file which is intended to be read into a speadsheet for further analysis.

The parameters that define thw model must be supplied in a text file, which by default is named basic.mod. To use a different file enter its name as the first argument when invoking the program. For example:

microsim myfile.mod

The number of iterations can be given as the second argument on the command line or included in the model definition file. If omitted altogether, 10 will be assumed.
