Generate diagrams from code or documentation
============================================

Diagrams related directly to the current code are a key part of any kind of technical documentation. As well being able to track down partial changes **inside** of those images along the code itself right out off the bat it's a huge improvement.


## Low level considerations

The tool is [PlantUML]http://plantuml.sourceforge.net) and the format usually used is **PNG**. The reason behind was that **metadata** information is embedded into those photos and it might be checked out images before even generating them. This way you can save a huge deal of time at big projects and avoid stesssing too much your *GIT* repositories with binaries.

That feature is not implemented at this pet project. So in order to prevent GIT from uploading too much binaries, 

