Templates to gather external information
========================================

The basic external information to be included is **GIT COMMIT HASH**. This way *code* and *documentation* are related by this piece of information.

![width=450px](../image/cmake.png)

<!---
@startuml cmake.png
left to right direction
(version.h) <|-- (template\nCMakeLists.txt)
(Doxyfile) <|-- (template\nCMakeLists.txt) : generate  
(header.tex) <|-- (template\nCMakeLists.txt)
(template\nCMakeLists.txt) <.. (version.h.in)
(template\nCMakeLists.txt) <.. (Doxyfile.in) : template 
(template\nCMakeLists.txt) <.. (header.tex.in)
(Doxyfile) <-- (header.tex) : details
(root\nCMakeLists.txt) <-- (version.h) 
(root\nCMakeLists.txt) <-- (Doxyfile) : Git\nCommit\nHash
(Doxyfile) <.. (several\nREADME.md) : include
note left of (root\nCMakeLists.txt): **binaries**\nmake\nmake install
note left of (root\nCMakeLists.txt): **documents**\nmake doc\nmake show 
@enduml
--->

As well information on the machine where **cmake** was invoked is collected.

**Note:** Take into account that **the last commit** information will be processed; if there are new changes not yet committed, they will be included anyway. So in order to generate *final official* documentation first commit all your changes, generate the documentation and, if needed, commit that generated document.

## GIT Commit Hash 

In order to add the specific **git commit hash** into code & documentation, *templates* are defined in the *template* folder for **Doxyfile**, **header.tex** & **version.h** files.

![width=400px](../image/version.png)

In order to **speed up** local compilations and let us hardcode our locally generated files, it's possible to instruct *cmake* to use this hardcoded header instead of usual GIT one.

The parameter to pass onto **cmake** is **VERSION_HARDCODED**:

          cmake <rest of options> -DVERSION_HARDCODED=TRUE ..
