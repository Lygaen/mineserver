<img src=".github/icon.png" align="right"
alt="MineServer Logo" width="256" height="256">
<h1>MineServer</h1>

A rewrite of the Java minecraft server (1.8.9) in C++ from scratch !

## Contents
- [Contents](#contents)
- [Goals](#goals)
- [Libraries](#libraries)

## Goals
The Mineserver project has simple goals : be fast, reliable and seemless (there should)
not be any difference from logging in a java minecraft server or the Mineserver project.

## Libraries
This project uses a few embedded libraries (included in the project) :
- [RapidJson](https://github.com/Tencent/rapidjson) one of the fastest json-parsing libs out there
- [libcpnet](https://github.com/soroush/libcpnet) a thin wrapper for POSIX and Windows's socket
- [fmt](https://github.com/fmtlib/fmt) an open-source, fast and safe formatting library
- [doctest](https://github.com/doctest/doctest/) a C++ testing framework that is the fastest at both compile-time and runtime
- [mINI](https://github.com/pulzed/mINI) a tiny header only c++ library for parsing `.ini` files

This list is exhaustive but may or may not change later as we gradually move to a standalone project.