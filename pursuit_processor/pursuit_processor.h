//
// Created by biba_bo on 2020-10-02.
//

#ifndef IRON_TURTLE_FOLOWING_PROCESSOR_H
#define IRON_TURTLE_FOLOWING_PROCESSOR_H

/** This is class for the processing auto moving of the iron turtle.
 * This has a start rectangle size and place and recalculate moving direction and speed by a new object rectangle and position.
 * This recalculations works in other thread and gets data from the process camera thread.
 * Possibly it can get data from other places.*/
class PursuitProcessor {
private:
public:
    PursuitProcessor();
    ~PursuitProcessor();


};


#endif //IRON_TURTLE_FOLOWING_PROCESSOR_H
