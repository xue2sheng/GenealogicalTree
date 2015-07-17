# Genealogical Tree 

## Summary 

Program should be able to **find all the descendant with name Bob for all the ascendants with name Will on any level of ancestry**. In order to present the capabilities of your app:

- implement the application to optimize the initialization time.
- application should have built in data about genealogical tree of people living in particular country.
- please generate a representative data that has sample people an relationships between them. Use all varieties of names (can be also generated) but also put two test names (Bob and Will) and connect them in different relationships.
- the application should posses tests that are checking possible edge cases and ensure the stability of the application.
- the designed data structure should ensure optimized search time on following fields: name, last name, date of birth and location.

## Approach 

In order to make it easier to mantain all the tasks and let us apply specific tools & languages for specific issues, a 'divide & conquer' policy has been taken based on **git submodules**: [Main](https://github.com/xue2sheng/GenealogicalTree), [Core](https://github.com/xue2sheng/GenealogicalTreeCore), [Persist](https://github.com/xue2sheng/GenealogicalTreePersist), [UI](https://github.com/xue2sheng/GenealogicalTreeUI) and [Base](https://github.com/xue2sheng/GenealogicalTreeBase). 

![width=150px](image/submodules.png)

<!---
@startuml submodules.png
rectangle Application {
 (Core) <|--|> (Persist)
} 
rectangle Framework {
 (Base\n--\nPlumbing tools) as (base)
 (UI\n--\nPorcelain tools) as (ui)
 (Main\n--\nDocs & Stats) as (main)
 (base) <..> (main) 
 (ui) <..> (main) 
 (ui) <..> (base) 
}
note "Reuse Framework\non different Apps" as N
N .. (Core) 
N .. (main) 
@enduml
--->

Instead of starting directly with the core problem, don't test thoroughly edge cases, leaping into too early optimization, don't document your results/decisions/mistakes and ending with an app that only run partially on your development environment, the **aproach** will be the opposite one. 

1. [Ensure a minimum of portability](doc/README.md) on different environments.
2. [Configure linux environment](base/vagrant/README.md) on your Windows or MacOSX box if needed.
3. [Generate diagrams from codex and documentation](image/README.md) to be able to track down all the changes.
4. [Use templates to gather external information](template/README.md) to document as much automatically as possible.
5. [Write tests](base/test/README.md) to cover your app and let you  optimize knowing you're not breaking previous development.
6. [Measure your application](core/optimize/README.md) in order to compare improvements/regressions during the optimization stage.
7. [Simulate your deployment infrastructure](base/mock/README.md) to hunt down integration issues as soon as possible.
8. [Solve the core problem](core/src/README.md) in the most simple and maintainable way at our disposal. 

![width=300px](image/approach.png)

<!---
@startuml approach.png
left to right direction
(Portability and\nDocumentation\n--\ndoc\README.md) as (Doc)
(Linux environment\n--\nbase/vagrant\README.md) as (Vagrant)
(Diagrams\n--\nimage\README.md) as (Image)
(Additional\ninformation\n--\ntemplate\README.md) as (Template)
(Test\nresources\n--\ncore/test\README.md) as (Test)
(Summary\n--\nREADME.md) as (Summary)
(Measure\napplication\n--\ncore/optimize\README.md) as (Measure) 
(Simulate\napplication\n--\nbase/mock\README.md) as (Mock) 
(Core\napplication\n--\ncore/src\README.md) as (Core)
(Doc) <.. (Summary)
(Vagrant) <.. (Summary)
(Template) <.. (Summary)
(Image) <.. (Summary)
(Test) <.. (Summary)
(Measure) <.. (Summary)
(Mock) <.. (Summary)
(Core) <.. (Summary)
(Doc) <|-- (Image)
(Doc) <|-- (Vagrant)
(Core) <|-- (Template)
(Core) <|-- (Test)
(Core) <|-- (Measure)
(Core) <|-- (Mock)
note right of (Doc): cmake\ndoxygen\nlatex\nmarkdown 
note left of (Vagrant): vagrant\nvirtual box\nkitematic\ndocker 
note left of (Image): cmake\nplantuml 
note left of (Measure): R scripts\nsystemtap 
note left of (Mock): go\nnodejs\njava 
note top of (Core): java\nGraph boost lib
note left of (Test): java\nTest boost lib
@enduml
--->

No doubt this approach is an overkill for a pet project but it's way more realistic for big, long ones. 
