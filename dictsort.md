Dictsort is a small C application that attempts to quickly order a password dictionary in manner which will present more likely passwords first, and less likely passwords last. It does this by observing the frequency by which certain characters follow others in the password dictionary, and then using this metric to sort the dictionary.

It can load, calculate character frequency, sort and write a password of dictionary of ~1 million passwords on my netbook (1.6ghz atom CPU) in 13 seconds.

It is by no means perfect, but is much better than nothing, and will hopefully speed up your brute-force attempts.