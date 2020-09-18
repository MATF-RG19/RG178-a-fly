# RG178-a-fly
A Fly

This is a small game where you need to destroy a spider web while avoiding getting caught by the spider.
I created the models for the spider and the fly using blender and exported them to .obj format. Then I created a parser for those .obj files that
generates finished .cpp and .h classes. Both the spider and fly classes have the same parent class Mesh.
The meshes are drawn triangle by triangle.

How to run and play: (Linux)
1. In the RG178-a-fly\VSProject\Projekat_iz_grafike\ folder there is a makefile
2. Run make command
3. Run a_fly file
4. To start the game, select any of the blue spheres with your mouse and press 'space'
5. Select the next sphere and hit 'space' again
6. Darker spheres need to be visited twice for them to disappear
7. Try to get as many points as possible before the spider gets you!

How to run and play: (Windows)
1. In the RG178-a-fly\VSProject\Debug\ folder there is an exe file
2. Run it
3. To start the game, select any of the blue spheres with your mouse and press 'space'
4. Select the next sphere and hit 'space' again
5. Darker spheres need to be visited twice for them to disappear
6. Try to get as many points as possible before the spider gets you!
