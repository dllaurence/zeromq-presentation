# Slides and example code to accompany a presentation on ZeroMQ

## By Dustin Laurence

## Given at SCALE on 3/9/2019

Repo: [https://github.com/dllaurence/zeromq-presentation](https://github.com/dllaurence/zeromq-presentation)

Slides: presentation.odp

Everything released under the MIT license--use as you wish except for claiming
you wrote it.

Abstract:

## ZeroMQ: Connect ALL The Things

> Writing applications in a multi-core, multi-threaded, multiprocess, networked
> world means communicating between many threads and processes over shared
> memory, IPC, and the network. This often involves multiple low-level libraries
> (e.g. most languages’ built-in threading, unix IPC, Berkeley sockets) with
> different programming paradigms, and may require a potential a scaling
> bottleneck in the form of a central server or broker to make it all manageable.
> ZeroMQ claims to be a better alternative, providing a single, higher-level
> message-passing toolkit across threads, processes, and networks, and languages,
> and specifically supports decentralized messaging. That should make it a
> slower, clunkier compromise for any one task, but it claims to be both better
> and easier to use for any one of those problems than a dedicated library.
> Extraordinary claims require extraordinary evidence, so we’ll examine how it
> compares to standard threading and networking facilities and see how easily we
> can just connect all the things like Lego bricks, regardless of type or
> underlying transport. Of course it wouldn’t be any fun without including some
> very informal performance smackdowns.
