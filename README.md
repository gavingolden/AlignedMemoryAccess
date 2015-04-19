# Unaligned-Memory-Access
This simple program tests runtime characteristics of accessing memory with(out) proper word alignment.

####Tests
<ol>
<li>Sequential memory access with non-word alignment.</li>
<li>Random access with non-word alignment.</li>
</ol>

####Usage
`<program> [, <repetitions>]`<br/>
Define **RANDOM** to perform random access test. Else, it defaults to sequential.

####Results
`-O2 optimizations, repetitions = 1.5 million`. <br/>
<img src="https://cloud.githubusercontent.com/assets/5760562/7217651/f40e3b2e-e608-11e4-9ab3-9670a7b4f73b.png" width="400" height="230"/>
<img src="https://cloud.githubusercontent.com/assets/5760562/7217653/197856f6-e609-11e4-8d5d-705e8c5fa457.png" width="400" height="230"/>

####Note
Sequential execution currently fails with -O3 optimizations enabled.
