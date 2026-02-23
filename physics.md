# What Is A Particle?

The first thing one needs to think about when creating a 2D physics simulation program is what a particle actually is and what kind of properties it has.
At a base level a particle can be comprised of:
* Position (2D Vector)
* Velocity (2D Vector)
* Force (2D Vector)
* Mass (Scalar)
* Charge (Scalar)

At its core these properties define all a particle is and how it will move through space.
A particle in a physics sense is a sizeless thing that can be described as a point.
You can further classify into different particles with actual size, making something more akin to a rigid-body object.
This is the approach to model rigid-body physics.

As an example, a (non-rotating) circle would have:
* All properties of the particle
* Radius
* Color


# How Do You Store A Particle?

This is the core first step of moving from abstract thinking of physics particles and objects to real implementation that can be run on a machine.
There are a few different approaches to storing particles/objects in a program that dictate the overall structure and execution of a program.

## 1. Object Oriented Design (OoD/OoP)

Object oriented programming is perhaps the most clean and understandable method of storing these physics objects in a program. It's quite literally in the name.
We can represent every particle as an instance of its "class". Furthermore, we can subdivide our kinds of objects using inheritance in subclasses.
The Physics Object can be the standard base class which Circle inherits from.

This makes simulation very intuitive as you can simply store an array of these objects (or pointers to them) which are iterated over every "frame" in the simulation to do force and movement calculations. Storing this array of objects is known as Array of Structures (AoS) design.
It's certainly an intuitive approach which made it great to start for building this simulation software. However, it does have its drawbacks...
Even if you go with the modular approach of storing pointers to base Physics Objects which can do polymorphism (doing identical operations regardless of actual class), calculating through this array of pointers becomes very scattered when you look at what the CPU is actually doing. The CPU computes by fetching "caches" of data, not individual variables. Each time it encounters a pointer in this cache, it must jump in the heap to what's pointed at and go through *all* properties stored there, even if it only needs a few select properties for a particular calculation. The whole point of the CPU caching is to optimize performance as the CPU assumes you will likely do computations with sequential members in memory. This is why CPU's absolutely love contiguous memory, meaning that if you were able to store this physics information contiguously, the simulation throughput would increase substantially...

## 2. Data Oriented Design (DoD)

### Entity-Component-System (ECS)