# Generate diagrams from code or documentation

Diagrams related directly to the current code are a key part of any kind of technical documentation. As well being able to track down partial changes **inside** of those images along the code itself right out off the bat it's a huge improvement.

## Way of working

**Note:** Use different names for any image you create

Generated images are saved at *image* folder, regardless from where defined at the project. So when you refer to them from **markdown** README.md files, you should user *relative paths*.

But **doxygen** *latex* documentation goes directly to that folder, so use just the name of the image.

## Low level considerations

The tool is [PlantUML](http://plantuml.sourceforge.net) and the format usually used is **PNG**. The reason behind was that **metadata** information is embedded into those photos and it might be checked out images before even generating them. This way you can save a huge deal of time at big projects and avoid stesssing too much your *GIT* repositories with binaries.

As well **SVG** format will be used by *Presentation* as *Sozi*. Being pure text are more *friendly* to *GIT* but being able to contain **code**, *GitHub* and other repositories prevent them from being renderized for your README.md files.
