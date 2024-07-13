# Decoda@k
Lua IDE and Debuger

原项目：https://github.com/unknownworlds/decoda

## 动机
自己觉得Decoda在windows环境下的的调试要好于其他工具，所以用得比较多，但原来的Decoda项目已经没人维护更新很多年了，所以把自己对Decoda的一些修改和更新分享下

## 修改内容
* 增加Lua 5.4版本的支持（由于自己的使用场景，跳过了5.3）
* 增加了自用的一些语法关键字高亮显示（class、switch、case等），不影响官方Lua版本代码的使用
* 增加Project目录管理，可增加子目录，方便项目管理
* 增加StepOut调试功能，方便Step调试时，从当前函数跳出
* 增加自动打开文档功能，下次打开Project时，自动打开上次关闭/退出时已打开的文档
* 优化界面代码逻辑

## 未来打算/可能
* 由于界面元素简单，目前没有打算增加多语言支持
* 增加编辑器utf8或多字节字符支持
* 由于Decoda是通过注入DLL的方式来实现Hook，不适合跨平台移植和远程调试，可能会考虑支持MobDebug

#

Decoda Source Code
==================================

BUILDING
========

Decoda is built from premake4 project files. premake4 can be downloaded from
http://industriousone.com/premake/download

Decoda has only been tested with the vs2013 target, however it may be possible
to compile Decoda with older versions. It is not currently possible to build
Decoda as a 64-bit executable or a non-Windows application.

LICENSE
=======

See COPYING.txt for the GNU GENERAL PUBLIC LICENSE

EXCLUDED CODE:  The code described below and contained in the Decoda Source Code
release is not part of the Program covered by the GPL and is expressly excluded
from its terms.  You are solely responsible for obtaining from the copyright
holder a license for such code and complying with the applicable license terms.


Lua library
---------------------------------------------------------------------------
libs/lua/*

Copyright (C) 1994-2006 Lua.org, PUC-Rio.  All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


wxWidgets and wxScintilla libraries
---------------------------------------------------------------------------
libs/wxWidgets/*
libs/wxScintilla/*

Copyright (C) 1998-2005 Julian Smart, Robert Roebling et al

				   WXWINDOWS LIBRARY LICENCE
 TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Library General Public Licence as published by
the Free Software Foundation; either version 2 of the Licence, or (at
your option) any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library
General Public Licence for more details.

You should have received a copy of the GNU Library General Public Licence
along with this software, usually in a file named COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA.

EXCEPTION NOTICE

1. As a special exception, the copyright holders of this library give
permission for additional uses of the text contained in this release of
the library as licenced under the wxWindows Library Licence, applying
either version 3.1 of the Licence, or (at your option) any later version of
the Licence as published by the copyright holders of version
3.1 of the Licence document.

2. The exception is that you may use, copy, link, modify and distribute
under your own terms, binary object code versions of works based
on the Library.

3. If you copy code from files distributed under the terms of the GNU
General Public Licence or the GNU Library General Public Licence into a
copy of this library, as this licence permits, the exception does not
apply to the code that you add in this way.  To avoid misleading anyone as
to the status of such modified files, you must delete this exception
notice from such code and/or adjust the licensing conditions notice
accordingly.

4. If you write modifications of your own for this library, it is your
choice whether to permit this exception to apply to your modifications. 
If you do not wish that, you must delete the exception notice from such
code and/or adjust the licensing conditions notice accordingly.


tinyxml library
---------------------------------------------------------------------------
libs/tinyxml/*

www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.


TreeListCtrl
---------------------------------------------------------------------------
src/Frontend/treelistctrl.h
src/Frontend/treelistctrl.cpp

Copyright (C) 2004 2004 Robert Roebling, Julian Smart, Alberto Griggio,
	Vadim Zeitlin, Otto Wyss


				   WXWINDOWS LIBRARY LICENCE
 TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Library General Public Licence as published by
the Free Software Foundation; either version 2 of the Licence, or (at
your option) any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library
General Public Licence for more details.

You should have received a copy of the GNU Library General Public Licence
along with this software, usually in a file named COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA.

EXCEPTION NOTICE

1. As a special exception, the copyright holders of this library give
permission for additional uses of the text contained in this release of
the library as licenced under the wxWindows Library Licence, applying
either version 3.1 of the Licence, or (at your option) any later version of
the Licence as published by the copyright holders of version
3.1 of the Licence document.

2. The exception is that you may use, copy, link, modify and distribute
under your own terms, binary object code versions of works based
on the Library.

3. If you copy code from files distributed under the terms of the GNU
General Public Licence or the GNU Library General Public Licence into a
copy of this library, as this licence permits, the exception does not
apply to the code that you add in this way.  To avoid misleading anyone as
to the status of such modified files, you must delete this exception
notice from such code and/or adjust the licensing conditions notice
accordingly.

4. If you write modifications of your own for this library, it is your
choice whether to permit this exception to apply to your modifications. 
If you do not wish that, you must delete the exception notice from such
code and/or adjust the licensing conditions notice accordingly.
