# Cooperative Pathfinding
Path-finding is one of the basic problems in the field of artificial intelligence and computer science in general. In the most basic variant, the task is to find a path for an agent through a given graph, so that the agent moves from vertex to vertex along the edges of the graph and ends in a specified goal vertex. 
A natural extension of the basic path-finding problem is multi-agent pathfinding. In this problem, we are given multiple agents, each with its own goal, and the task is to again guide them toward their goals along the edges of a given graph. Agents have knowledge of each other’s plans, which allows them to cooperate. The amount of cooperation required between agents depends on the problem instance, and it can vary from very high on instances where the number of agents is close to the number of available vertices to very low on instances where the number of agents is small relative to the size of the graph and there is enough space between the agents.

## Table of Content
- [Cooperative Pathfinding](#cooperative-pathfinding)
	- [Table of Content](#table-of-content)
	- [Instalation](#instalation)
		- [Prerequirements](#prerequirements)
		- [Install](#install)
## Instalation
### Prerequirements
For the creation build from the sources, you will needed in CMake. This util will configure a project structure for any IDE. You can download it [here](https://cmake.org/download/).

### Install
- Go to the path where you want to clone
	> \> cd <your_path\>
- Clone the repository
	> \> git clone https://github.com/JuiceFV/Cooperative_Pathfinding.git Coop_Pathfinding
- Go to the working derictory, which we have been cloned on previous step
	> \> cd Coop_Pathfinding
	- Now you have a such hierarchy
  	![Working directory tree](https://user-images.githubusercontent.com/35202460/68800408-157cf700-066b-11ea-852c-047506d1e9f9.png)
- Now let's create folder called "build"
	> \> mkdir build
- And going there
	> \> cd build
- Now, let's build a project for any IDE that you want
  - **If you woud like to pick another IDE**
	- You can check the list of the available IDE
		> \> cmake -G
	- Now you will see the list of IDE. You can see the star beside an IDE name, it's a current IDE and the build will be making for this IDE. You can pick any IDE, which you like by typing this command. You can find more information [here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html)
		> \> cmake -G <ide-name\> ..
	- The default build type is Release type, if you would like to pick the other type just use this command
		> \> cmake -G <ide-name\> -DCMAKE_BUILD_TYPE=Debug ..
  - **If the current IDE suit you**
    - Just use this command. And as in the previous step you can change the build type
		> \> cmake [optional: -DCMAKE_BUILD_TYPE=Debug] ..
- CMake will configure the project in the "build" and you can work with it.
	

		