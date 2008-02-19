/*
    phifs.inl
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#define READ_OBJ_L_THUNK_01 { RChunk chunk; if(chunk.Open(find,EOwnerThread)==KErrNone) { iBuffer().iSize=chunk.Size(); chunk.Close(); } }
READ_OBJ_L(Chunk)
#undef READ_OBJ_L_THUNK_01
#define READ_OBJ_L_THUNK_01
READ_OBJ_L(Library)
READ_OBJ_L(LogicalChannel)
READ_OBJ_L(LogicalDevice)
READ_OBJ_L(Mutex)
READ_OBJ_L(PhysicalDevice)
READ_OBJ_L(Process)
READ_OBJ_L(Semaphore)
READ_OBJ_L(Server)
READ_OBJ_L(Thread)
#undef READ_OBJ_L_THUNK_01
