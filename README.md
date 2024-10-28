I misunderstood the instructions at first and accidentally made a version of this where the dequeuement of tickets are based on their individual clocks instead of a constant every-minute rule. 
So in that version, tickets would be dequeued when its been a minute since their time of creation.

You can try that version out by addding true as a parameter on the updateQueue() method somewhere around line 362.
