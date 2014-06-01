Testing damping on the voltmeter, stopping some of the intertia from the heavy needle.

Damping is achieved by applying a reverse voltage (proportional to the distance) for a brief period of time after a specified delay from initial movement (the function in the file is fairly self-explanatory).
These values (in the file as #defines) can be tuned to improve the effect (may have to change them if we trim down the needle).

'tick.ino' contains the damped needle move function, as well as tests.