# Traffic Intersection Camera Installation - Vertex Cover

The project is to help the local police department with their installation of security cameras at traffic intersections. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring. This is a typical Vertex Cover Problem

## a1

Given the streets represented by their GPS coordinates, calculates the intersections and the edges between the intersections. Written in Python.

## a2

Calculates the shortest path using Bellman-Ford algorithm in C++.

## a3

Launches different processes for a random streets generator and the componets in a1 & a3, and connect these processes using Linux C pipes.

## a4

Creates a polynomial reduction of the decision version of Vertex Cover to CNF-SAT. Implements the reduction and use MiniSat as a library to solve the problem.

## proj

Use another two naive approximation algorithms to solve the Vertex Cover problem besides CNF-SAT. Executes these methods through multiple threads, and analyzes the degree of approximation, speed and scalability of these algorithms.