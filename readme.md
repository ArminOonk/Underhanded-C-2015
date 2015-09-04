http://www.underhanded-c.org/

The 2015 Challenge: Faking Fissile Material

Two countries, the Peoples Glorious Democratic Republic of Alice and the Glorious Democratic Peoples Republic of Bob, have agreed to a nuclear disarmament treaty. In practice, this is implemented by nuclear inspectors visiting each country and verifying the presence of fissile material such as Plutonium in a warhead, at which point the warhead can be destroyed.

Ideally, the inspectors would subject a warhead to a scan and observe a graph such as a radiogram or a gamma ray spectrum of the object under test, so that they can confirm the warhead contains what it is supposed to contain. But both the PGDRA and the GDPRB are dissatisfied with this approach because the results of these scans contain sensitive information about their nuclear programs and the design of their nuclear weapons. However, each country wants to ensure that the other country is dismantling real nuclear warheads, and not fakes.

To this end, the two countries agree to build a fissile material detector with an “information barrier”—-essentially a computer program that will take the result of a scan, determine if it matches some reference pattern, and output only a “yes” or “no.”

Your goal is to write the function that compares a test measurement to a reference pattern:

int match( double * test, double * reference, int bins, double threshold )
…where test and reference are 1-dimensional arrays of width bins, that encode a gamma ray spectrum of a test warhead and a reference warhead, respectively. Your detector should output a 1 (“yes”) if these two arrays are sufficiently similar, and 0 (“no”) otherwise; you should use the parameter threshold to adjust the sensitivity of the match.

You are free to use any formula to decide if test[] and reference[] are a close match, as long as it is justifiably accurate: i.e., that two very similar signals match for a reasonable threshold, and two clearly distinguishable signals fail to match for some threshold. You will get more points for using a common comparison formula or detector structure; something weird and ad-hoc may be suspicious and hard to justify.

Here’s what you can assume about the data:

Certain elements, including plutonium, will produce a spectrum with peaks (significantly large values) in some of the array elements;
The fissile material in a warhead has a pattern of peaks that is essentially impossible to counterfeit with other elements—-with bogus material certain peaks will not be as high, and spurious peaks will be present.
A complete absence of interesting elements produces a fairly boring baseline spectrum that is not secret; you can assume the existence of a global array double baseline[] that holds the expected values of this spectrum, which you may use in your detector.
In all observations, the measured spectra contain some random noise and will not match exactly.
Due to registration or calibration issues, a warhead may need to be tested multiple times against several reference patterns.
The reference pattern or patterns are either stored securely, or determined on the spot by aiming the detector hardware at a reference warhead.
Reference patterns are completely trustworthy and can not be doctored in advance.
The Underhanded Part

Somehow, match() should return a “yes” even when fissile material is missing, if triggered by certain circumstances that can be engineered by the host country. To be more specific, it should somehow return 1 for a test[] array that is so unlike reference[] that the detector should output 0. Moreover, this can’t be a bad detector overall: it must correctly and consistently output 0 for arbitrary test[] vectors that don’t match reference[]. Only under unusual special conditions should this false match arise.

The conditions that trigger the underhanded behavior are up to you. They can be caused by some characteristic of the test[] data, some quirk of the program's environment, etc. An entry is worth more points if the underhanded conditions are easily engineered, rarely happen by accident, and do not result in suspicious code.

Scoring and Extra Points

In general, submissions are worth more points if they are shorter and more readable, because it is more impressive to hide a bug in short, readable code.
Errors based on human perception, like mistaking an l for a 1, are worth just as much as “hard” errors based on pointer abuse or little-endian weirdness or quirks of C function calls. The goal is a clever vulnerability that passes visual inspection, whatever the mechanics of the underlying bug.
Bugs are worth more points if, once discovered, they are plausibly deniable as an innocent programming error.
Errors are worth more points if they remain innocent-looking under syntax coloring.
Errors may be worth fewer points if they are processor or OS dependent, but only if we have to scavenge a system to test your bug. If your bug works specifically under GNU/Linux, don’t worry about it. If it works specifically under x86, no problem. If it only works on BeOS R5 on a dual G3 box when all four MIDI ports are active, then no.
As always, extra points are awarded for humorous, spiteful, or ironic bugs, such as error-prone behavior in an error-checking routine.