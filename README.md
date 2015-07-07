Genealogical Tree
=================

## Summary

Program should be able to **find all the descendant with name Bob for all the ascendants with name Will on any level of ancestry**. In order to present the capabilities of your app:

- implement the application to optimize the initialization time.
- application should have built in data about genealogical tree of people living in particular country.
- please generate a representative data that has sample people an relationships between them. Use all varieties of names (can be also generated) but also put two test names (Bob and Will) and connect them in different relationships.
- the application should posses tests that are checking possible edge cases and ensure the stability of the application.
- the designed data structure should ensure optimized search time on following fields: name, last name, date of birth and location.

## Approach

Instead of starting directly with the problem core, don't test thoroughly edge cases, leaping into too early optimization, don't document your results/decisions/mistakes and ending with an app that only run partially on your development environment, the **aproach** will be the opposite one. 

1. Ensure a minimum of portability on different environments as hinted in the next section.
2. [Generate diagrams from code/documentation](image/README.md) to be able to track down all the changes.
3. [Use templates to gather external information](template/README.md) to document as much automatically as possible.
4. [Write tests](test/README.md) to cover your app and let you  optimize knowing you're not breaking previous development.
5. [Measure your application](optimize/README.md) in order to compare improvements/regressions during the optimization stage.
6. [Solve the core problem](src/README.md) in the most simple and maintainable way at our disposal. 

![width=400px](image/approach.png)

<!---
@startuml approach.png
left to right direction
(Image\napplication\n--\nimage\README.md) as (Image)
(Template\napplication\n--\ntemplate\README.md) as (Template)
(Test\napplication\n--\ntest\README.md) as (Test)
(Portability &\nDocumentation\n--\nREADME.md) as (Doc)
(Measure\napplication\n--\noptimize\README.md) as (Measure) 
(Core\napplication\n--\nsrc\README.md) as (Core)
(Template) <.. (Doc)
(Image) <.. (Doc)
(Test) <.. (Doc)
(Measure) <.. (Doc)
(Core) <.. (Doc)
(Core) <|-- (Template)
(Core) <|-- (Test)
(Core) <|-- (Measure)
note top of (Doc): cmake\ndoxygen\nlatex\nmarkdown 
note left of (Image): cmake\nplantuml 
note left of (Measure): R scripts 
note "graph & test\nboost lib" as boost
boost .. (Core)
boost .. (Test)
@enduml
--->

No doubt this approach is an overkill for a pet project but it's way more realistic for big, long C++ ones. 

# Portability and documentation 

A Modern C++ GNU compiler, g++ 4.9.2 or above, and a recent cmake, 3.1 or above, are the minimum. As well a valid *boost* library is supposed to be installed.

### DEB Linux Type

Regarding to documentation, *doxygen*, *latex*, *graphviz* and *plantuml.jar* are needed. For example, if you work with *#Xubuntu** 15.04 or its **Docker** equivalent, the following commands might do the trick for you:
    
          sudo apt-get -y install git build-essential libboost-all-dev 
          sudo apt-get -y install doxygen doxygen-latex openjdk-8-jdk graphviz
          sudo add-apt-repository -y ppa:george-edison55/cmake-3.x
          sudo apt-get -y update
          sudo apt-get -y install cmake
          sudo apt-get -y upgrade
          sudo mkdir /opt/plantuml && sudo chmod a+wr /opt/plantuml
          wget http://sourceforge.net/projects/plantuml/files/plantuml.jar/download \
               -O /opt/plantuml/plantuml.jar
          
If you want to use latest *compiler*, you can use [an extra repository](http://askubuntu.com/questions/618474/how-to-install-the-latest-gcurrently-5-1-in-ubuntucurrently-14-04): 

          sudo add-apt-repository ppa:ubuntu-toolchan-r/test
          sudo apt-get update
          sudo apt-get install gcc-5 g++-5

But then you might want to [compile newer *boost* libraries](http://www.boost.org/doc/libs/1_58_0/more/getting_started/unix-variants.html) with that compiler.


### RPM Linux type

Another typical Linux platform is **CentOS**. Their [*gcc*](https://www.vultr.com/docs/how-to-install-gcc-on-centos-6) and [*cmake*](http://www.linuxfromscratch.org/blfs/view/svn/general/cmake.html) are very conservative, even for *CentOS 7*, so compile newer ones from **source code** might be a possibility:

#### CMake: 

Download *tar.gz* with latest version and *untar* its source code

          cd <building directory>
          ./bootstrap --prefix=/opt/cmake --mandir=/opt/cmake/man --docdir=/opt/cmake/doc
          make
          sudo make install

#### Compiler

It's going to take long so try to use all the cores you got

          mkdir ~/sourceInstallations
          cd ~/sourceInstallations
          svn co svn://gcc.gnu.org/svn/gcc/tags/gcc_5_1_0_release/
          cd gcc_5_1_0_release/ 
          ./contrib/download_prerequisites
          cd ~/sourceInstallations
          mkdir gcc_5_1_0_release_build/
          cd gcc_5_1_0_release_build/
          ../gcc_5_1_0_release/configure --enable-lenguages=c,c++ --prefix=/opt/gcc \
                                         --program-suffix=-5 --disable-multilib 
          make -j <number of cores> 
          sudo make install

          cd /usr/bin
          sudo ln -s /opt/gcc/bin/gcc-5 gcc-5
          sudo ln -s /opt/gcc/bin/g++-5 g++-5

#### Boost

A long compilation that needs to [be told where](http://www.boost.org/build/doc/html/bbv2/overview/configuration.html) to get the proper [toolset](http://hnrkptrsn.github.io/2013/02/26/c11-and-boost-setup-guide/). 

          cd <folder with source code>
          cp tools/build/example/user-config.jam .

Don't forget to edit  *user-config.jam* to point to **g++-5**, i.e., *using gcc : 5 : /opt/gcc/bin/g++-5*

          using gcc : 5 : /opt/gcc/bin/g++-5
          :
          <dll-path>/opt/gcc/lib64:/opt/gcc/boost/lib
          <harcode-dll-paths>true
          <cxxflags>-std=c++14
          <cxxflags>-Wl,-rpath=/opt/gcc/lib64:/opt/gcc/boost/lib
          <linkflags>-rpath=/opt/gcc/lib64:/opt/gcc/boost/lib
          ;

**Note:** Not all the targets might be created. If some of the missing ones are required for your apps, try to hack *boost* compilation scripts.

          ./bootstrap.sh --prefix=/opt/gcc/boost --with-toolset=gcc
          sudo ./b2 -j8 install toolset=gcc-5 --prefix=/opt/gcc/boost \
                    threading=multi define=BOOST_SYSTEM_NO_DEPRECATED stage release

**Note:** Take into account when compile with 'g++-5' on one Linux platform that got its own previous compiler version, you should let know to the **linker** where to get 'g++-5' libraries. Try to avoid **LD_LIBRARY_PATH** and use instead **RPATH**:

          g++-5 -pthread —std=c++14 -Wl,-rpath=/opt/gcc/lib64 <rest of options> 

In case of your linking against *boost* generates too many *auto_ptr* deprecated warnings:

          g++-5 -pthread -std=c++14 -Wno-deprecated -Wl,-rpath=/opt/gcc/lib64:/opt/gcc/boost/lib \
                -I/opt/gcc/boost/include -L/opt/gcc/boost/lib -lboost_program_options <rest of options.

**Hint:** If you generate those *cmake*, *gcc* and *boost* on one machine and then copy them onto another, remember that there is [**soft links** involved](http://www.golinuxhub.com/2013/12/how-to-preserve-symbolic-links-with-tar.html).

### OSX type 

In order to use *GNU* compiler instead of *XCode* **clang** one, there are several options. The one followed for this project was [Homebrew](http://brew.sh).

**Note:** If you happen to work with *OSX* and *Homebrew*, don't forget to compile **boost** with the previous **gcc** compiler, not with the default *clang* one:

          brew install gcc
          brew install boost --cc=gcc-5

### Windows type

As well there are several options to get your *GNU* chaintool ready on windows instead of *Visual Studio*. For example, [Git](https://git-scm.com/download/win) and [MinGW](http://nuwen.net/mingw.html). 

Another option might be following [MSYS2](https://github.com/gtorrent/gtorrent-core/wiki/Building-on-Windows) instruction:

          pacman -Syu 
          pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-pkgconf make
          pacman -S git mingw-w64-x86_64-cmake-git

## Generate binaries & documentation

Usual commands:

          mkdir build
          cd build
          cmake ..
          make
          make doc

Optionally you can invoke *make install* to install binaries or *make install_doc* / *make show* to install / preview documentation.

**Note:** If you happen to work with *OSX* and [Homebrew](http://brew.sh), don't forget to invoke *cmake* pointing to the **GNU** compiler:

          cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-5 ..

**Note:** If you happen to work with *Windows* and [Git](https://git-scm.com/download/win)/[MinGW](http://nuwen.net/mingw.html), don't forget to invoke *cmake* pointing to the **GNU** generator:

          cmake -G "MSYS Makefiles" ..

As well a script, called **show** or something similar, will be created in your *home* directory as a shortcut for generating & viewing documentation. Don't hesitate to use it as a *template* for your specific environment.

## Generate only documentation 

Similar commands to the previous ones, just the compiler is not required:

          mkdir build
          cd build
          cmake -DONLY_DOC=TRUE ..
          make doc

**Note:** If you happen to work with *Windows* and [Git](https://git-scm.com/download/win)/[MinGW](http://nuwen.net/mingw.html), don't forget to invoke *cmake* pointing to the **GNU** generator:

          cmake -G "MSYS Makefiles" -DONLY_DOC=TRUE ..

**Note:** If your **make** utility is not installed in the default place, define *CMAKE_BUILD_TOOL* 

          cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TOOL=<your location> -DONLY_DOC=TRUE ..

As well, if you installed the documentation utility with **make show**, you're supposed to able to recreate and view that documentation PDF though usual *ssh* connection with enabled X11:

          ssh -X <user>@<location> "./show"

**Note:** By default **make install_doc** or **make show** copy the documentation *PDF* with the default project name in your **home** directory. You can define that target file with:

          cmake <rest of options> -DDOC_PDF=<your path & name, ending in .pdf> ..

## IDE hints

To use **NetBeans** don't forget to configure a *cmake* project with *custom* **build** folder. Add at that moment any extra customization in the command line used by *cmake* instruction. For example:

 - -DCMAKE_CXX_COMPILER=g++-5 for **OSX**
 - -DONLY_DOC=TRUE for only documentation on **Linux/OSX**
 - -G "MSYS Makefiles" for **Windows**
 - -G "MSYS Makefiles" -DONLY_DOC=TRUE for only documentation on **Windows**

**Note:** If you happen to use *jVi* plugin on *OSX*, don't forget to use "-lc" instead of just "-c" for its /bin/bash flag. 

## Development details

In order to generate binaries & documentation, the following versions were used:

### For code

Pay attention to *cmake* and *gcc* versions. A minimum is required to work on several O.S. using modern C++. Feel free to locally hack **CMakeLists.txt** to meet your needs.

#### Linux ( Xubuntu 15.04 )

- **cmake** *3.2.2*
- **gcc** *4.9.2*
- **boost** *1.55*

#### OSX ( Yosemite 10.10.4 )

- **cmake** *3.2.2*
- **gcc** *5.1*
- **boost** *1.58*
        
#### Windows ( Win7 x64 )

 - **cmake** *3.3.0*
 - **gcc** *5.1*
 - **boost** *1.58*

### For documentation

Environment variables to locate PlantUML *jar* and default *PDF* viewer can be defined to overwrite default values. See **CMakeLists.txt** for further information on your platform.

#### Linux

- **doxygen** *1.8.9.1*
- **latex/pdfTeX** *2.6-1.40.15*
- **graphviz/dot** *2.38.0*
- **java/plantuml** *1.8.0_45/8026*

#### OSX

- **doxygen** *1.8.9.1*
- **latex/pdfTeX** *2.6-1.40.15*
- **graphviz/dot** *2.38.0*
- **java/plantuml** *1.8.0_40/8026*

#### Windows

 - **doxygen** *1.8.9.1*
 - **latex/pdfTeX** *2.9.5496-1.40.15*
 - **graphviz/dot** *2.38.0*
 - **java/plantuml** *1.8.0_45/8026*

**Note:** Don't forget configure *Doxyfile* and *CMakeLists.txt* to use **README.md** as *Main Page* for **latex** documentation. 

