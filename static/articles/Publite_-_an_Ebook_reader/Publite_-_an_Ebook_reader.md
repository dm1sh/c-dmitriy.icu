I like reading books. Really like. But I can't bring paper ones everywhere I want and not all books are available in the local library. So I have to use my phone or laptop. Until now I tried lots of various ebook readers, but no one of them satisfied all my requirements. Some were too complicated for just reading text, some, on the other hand, lacked functionality, and some just had an awful design.

After some searches, I understood, that nothing can fit my needs but my own tool. So I created Publite — a web application for book reading. For it I also created a backend for converting EPUB and FB2 files to HTML After adding the book, you can read it offline. The book is split into pages fitting window size.

The first version of the application is already available at [publite.dmitriy.icu](https://publite.dmitriy.icu). It is a React.JS SPA using LocalStorage as book storage. To provide simple routing without dramatic bundle size growth I used [Wouter](https://github.com/molefrog/wouter) instead of React-Router.

But this implementation is not ideal. First of all, it would be difficult and ugly to add server synchronization to the current data flow. Secondly, pagination which I stole from [this article](https://blog.cacoveanu.com/2020/2020.08.14.pagination_in_ebooks.html) is too inefficient for my case.

I already started solving the first problem by switching from React context to fetches caught by ServiceWorker. Books will be stored in IndexedDB. I also added caching of static assets with browser Cache API. It allows me to turn the app into PWA and even publish it to Play Market (if I won't abandon this project).

As for pagination, it is also very important and I have a couple of ideas on how to improve it, but so far, we have to deal with it. Now I'm applying to university and traveling across Belarus. So I don't have much time for this project.

![Book adding screenshot](/articles/Publite_-_an_Ebook_reader/LDwoOj469e.jpg)
*Book adding screenshot*

![Book list screenshot](/articles/Publite_-_an_Ebook_reader/88PVXytCcA.jpg)
*Book list screenshot*

![Book view screenshot](/articles/Publite_-_an_Ebook_reader/qMCsbHZi67.jpg)
*Book view screenshot*
