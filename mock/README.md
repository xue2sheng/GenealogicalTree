# Simulate deployment infrastructure 

In order to make easy start testing even before all the components are fully developed, some mechanism to integrate **mocks** is required.

A humble [electron](http://electron.atom.io) application to supervise different mock services that simulate the real project.

Those mock services might be just [Docker](https://www.docker.com) containers with a bunch of **nodejs** and/or **go** scripts.