# Unaligned-Memory-Access
This simple program tests runtime characteristics of accessing memory with(out) proper word alignment.

####Tests
<ol>
<li>Sequential memory access with non-word alignment.</li>
<li>Random access with non-word alignment.</li>
</ol>

All tests were performed with ***g++ -O2*** optimizations.

####Usage
Compile with **RANDOM** defined to compile the random access test. Else, it defaults to sequential.

####Results


####Note
Sequential execution currently fails with -O3 optimizations enabled.
