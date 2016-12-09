#!/usr/bin/env python

"""
	made by Alessandro Molina <amol@sig11.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
"""

import pygtk
pygtk.require('2.0')
import gobject
import pango
import gtk
import gtk.glade
import diacanvas as dia
import pickle

roomNumber = 0
selectedRoom = None
selectedDirection = None

def ChangeRadio(widget):
	global selectedDirection
	if(widget.get_active()):
		selectedDirection = widget.get_label()
		print selectedDirection

def NewRoom(widget):
	global selectedRoom
	if(selectedRoom != None):
		window = widgetTree.get_widget('w_newroom')
		window.show()

def DeleteRoom(widget):
	global selectedRoom
	global map
	if(selectedRoom != None):
		map.delRoom(selectedRoom)

	
def MakeRoom(widget):
	global selectedRoom
	global selectedDirection
	global map
	box = MUDRoom()
	
	if(selectedRoom != None):
		dir = selectedDirection
		if(dir=="Nord"):
			box.move(selectedRoom.x, selectedRoom.y-45)
			box.setLevel(selectedRoom.level)
		elif(dir=="South"):
			box.move(selectedRoom.x, selectedRoom.y+45)
			box.setLevel(selectedRoom.level)
		elif(dir=="East"):
			box.move(selectedRoom.x+45, selectedRoom.y)
			box.setLevel(selectedRoom.level)
		elif(dir=="West"):	
			box.move(selectedRoom.x-45, selectedRoom.y)
			box.setLevel(selectedRoom.level)
		elif(dir=="Up"):
			box.move(selectedRoom.x+15, selectedRoom.y-15)
			box.setLevel(selectedRoom.level+1)
		elif(dir=="Down"):
			box.move(selectedRoom.x-15, selectedRoom.y+15)
			box.setLevel(selectedRoom.level-1)
		map.linkRooms(box, selectedRoom)
		map.addRoom(box)	

	window = widgetTree.get_widget('w_newroom')
	window.hide()


def ChooseRoom(room):
	nameEntry = widgetTree.get_widget('RoomNameEntry')
	descText = widgetTree.get_widget('RoomDescText')
	roomList = widgetTree.get_widget('RoomList')
	roomNEntry = widgetTree.get_widget('roomnentry')
	HEntry = widgetTree.get_widget('hentry')
	global selectedRoom	
	selectedRoom = room
	
	nameEntry.set_text(room.Name)
	roomNEntry.set_text(str(room.num))
	descText.get_buffer().set_text(room.Desc)
	HEntry.set_text(str(room.level))

def UpdateRoomName(widget):
	if(selectedRoom != None):
		selectedRoom.Name = widget.get_text()

def UpdateRoomDesc(widget, prot, borp):
	if(selectedRoom != None):
		selectedRoom.Desc = widget.get_buffer().get_text(widget.get_buffer().get_start_iter(), widget.get_buffer().get_end_iter())

def UpdateRoomDescB(widget, prot):
	if(selectedRoom != None):
		selectedRoom.Desc = widget.get_buffer().get_text(widget.get_buffer().get_start_iter(), widget.get_buffer().get_end_iter())


def open_file(widget, dialog):
	global map
	map.load(dialog.get_filename())
	dialog.destroy()

def on_apri_active(widget):
	print "APRI!"
	filesel = gtk.FileSelection(title="Open MUD map")
	filesel.cancel_button.connect("clicked", lambda w: filesel.destroy())
	filesel.ok_button.connect("clicked", open_file, filesel)
	filesel.show()

def save_file(widget, dialog):
	global map
	map.save(dialog.get_filename())
	dialog.destroy()

def on_salvan_active(widget):
	filesel = gtk.FileSelection(title="Save MUD map")
	filesel.cancel_button.connect("clicked", lambda w: filesel.destroy())
	filesel.ok_button.connect("clicked", save_file, filesel)
	filesel.show()


class MUDZone:
	def __init__(self):
		global canvas
		firstroom = MUDRoom()
		firstroom.Name = "STARTING LOCATION"
		firstroom.move(400, 400)
		self.roomList = [ firstroom ]
		canvas.root.add(firstroom)
		
	def addRoom(self, room):
		global canvas
		self.roomList.append(room)
		print room.Name
		print room.x, room.y
		canvas.root.add(room)

	def delRoom(self, room):
		self.roomList.remove(room)
		canvas.root.remove(room)

	def linkRooms(self, roomA, roomB):
		global canvas
		roomA.linkes.append(roomB.num)
		link = dia.CanvasLine()
		canvas.root.add(link)
		roomA.connect_handle(link.handles[1])
		roomB.connect_handle(link.handles[0])
		if(roomA.level>roomB.level):
			link.set(color=roomA.getColorForLevel(roomA.level))
		else:
			link.set(color=roomB.getColorForLevel(roomB.level))

	def findRoomN(self, roomN):
		for room in self.roomList:
			if(room.num == roomN):
				return room
		return None

	def save(self, file):
		print "Saving on file", file
		f = open(file, 'w+b')
		p = pickle.Pickler(f, 1)
		data = []
		for room in self.roomList:
			print 'Saving room: ' + str(room.num)
			data.append(room.getState())
		p.dump(data)
		f.close()

	def load(self, file):
		global roomNumber
		global selectedRoom
		global canvas
		f = open(file, 'r')
		p = pickle.Unpickler(f)
		data = p.load()
		f.close()
		for room in self.roomList:
			self.delRoom(room)
		roomNumber=0
		selectedRoom=None
		for state in data:
			room = MUDRoom()
			room.setState(state)
			if(roomNumber<room.num):
				roomNumber = room.num
			self.addRoom(room)
		for room in self.roomList:
			print 'Starting to link room ' + str(room.num)
			for linkN in room.linkes:
				roomB = self.findRoomN(linkN)
				if(roomB != None):
					print 'Linking ' + str(room.num) + ' to ' + str(roomB.num)
					link = dia.CanvasLine()
					canvas.root.add(link)		
					room.connect_handle(link.handles[1])
					roomB.connect_handle(link.handles[0])
					if(room.level<roomB.level):
						link.set(color=room.getColorForLevel(room.level))
					else:
						link.set(color=roomB.getColorForLevel(roomB.level))
		

class MUDRoom(dia.CanvasBox, dia.CanvasAbstractGroup):
	def __init__(self):
		global roomNumber
		self.__gobject_init__()

		self.x=0
		self.y=0
		self.num = roomNumber = roomNumber+1
		self.text = dia.CanvasText()
		self.changable=1
		self.Name = "UNKNOWN ROOM"
		self.Desc = "Empty Description..."
		self.level = 25
		self.linkes = []

		self.set(height=25, width=25)
		self.add_construction(self.text)
		font = pango.FontDescription('sans 20')
		def text_changed(text_item, text, me):
			if(me.changable):
				me.changable=0
				print 'rechanging text'
				text_item.set(text=self.num)
				me.changable=1
			else:
				pass
		self.text.set(text=self.num, font=font, width=25, height=25)
		self.text.connect('text_changed', text_changed, self)
		self.text.move(0, 0)

	def getColorForLevel(self, level):
		if(level>0 and level<50):
			if(level==25):
				return dia.color(0, 0, 0, 255)
			elif(level<25):
				return dia.color(255, 0, 0, level*10)
			elif(level>25):
				return dia.color(0, 255, 0, (50-level)*10)
		else:
			print 'ERROR level too low'


	def setLevel(self, level):
		if(level>0 and level<50):
			self.level=level
			self.set(color=self.getColorForLevel(level))
		else:
			print 'ERROR level too low'

	def getState(self):
		return (self.x, self.y, self.num, self.Name, self.Desc, self.level, self.linkes)

	def setState(self, state):
		self.x,self.y,self.num,self.Name,self.Desc,self.level,self.linkes = state
		x = self.x
		y = self.y
		self.x = 0
		self.y = 0
		self.move(x, y)
		self.setLevel(self.level)
		self.text.set(text=self.num)

	def on_event(self, event):
		if(event.type == dia.EVENT_BUTTON_PRESS):
			ChooseRoom(self)
		return dia.CanvasBox.on_event(self, event)

	def on_update(self,  affine):
		dia.CanvasBox.on_update (self, affine)
		self.update_child(self.text, affine)

	def on_move(self, x, y, interactive):
		self.x=self.x+x
		self.y=self.y+y
		return dia.CanvasBox.on_move(self, x, y, interactive);

	def on_groupable_get_iter(self):
		return self.text

	def on_groupable_remove (self, item):
        	pass

	def on_groupable_next (self, iter):
		return None

	def on_groupable_value (self, iter):
		return iter

	def on_groupable_length (self):
		return 1

	def on_groupable_pos (self, item):
		if item == self.text:
			return 0
		else:
			return -1

gobject.type_register (MUDRoom) 
dia.set_callbacks (MUDRoom)
dia.set_groupable (MUDRoom)

def quit_handler(widget):
	gtk.main_quit()

def ShowAboutWin(widget):
	window = widgetTree.get_widget('aboutwin')
	window.show()

def HideAboutWin(widget):
	window = widgetTree.get_widget('aboutwin')
	window.hide()

def LinkRooms(widget):
	global widgetTree
	global map
	RoomAID = widgetTree.get_widget('roomLinkEntryA').get_text()
	RoomBID = widgetTree.get_widget('roomLinkEntryB').get_text()
	roomA = map.findRoomN(int(RoomAID))
	roomB = map.findRoomN(int(RoomBID))
	map.linkRooms(roomA, roomB)

widgetTree = gtk.glade.XML("clexmap.glade")
MainWindow = widgetTree.get_widget('mainwin')

canvas = dia.Canvas()
map = MUDZone()

view = dia.CanvasView(canvas)
mainbox = widgetTree.get_widget('canvaswindow')
view.show()
mainbox.add(view)

widgetTree.signal_connect('gtk_main_quit', quit_handler)
widgetTree.signal_connect('update_room_name', UpdateRoomName)
widgetTree.signal_connect('update_room_desc', UpdateRoomDesc)
widgetTree.signal_connect('update_room_descB', UpdateRoomDescB)
widgetTree.signal_connect('make_new_room', NewRoom)
widgetTree.signal_connect('create_new_room', MakeRoom)
widgetTree.signal_connect('radio_button_click', ChangeRadio)
widgetTree.signal_connect('del_room', DeleteRoom)
widgetTree.signal_connect('on_apri1_activate', on_apri_active)
widgetTree.signal_connect('on_salva_come1_activate', on_salvan_active)
widgetTree.signal_connect('on_about1_activate', ShowAboutWin)
widgetTree.signal_connect('hide_about_win', HideAboutWin)
widgetTree.signal_connect('on_link_ok', LinkRooms)
gtk.main()
