/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gelide
 * Copyright (C) 2008 - 2011 Juan Ángel Moreno Fernández
 *
 * gelide is free software.
 *
 * You can redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * gelide is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gelide.  If not, see <http://www.gnu.org/licenses/>
 */

#include "system_manager.hpp"

void CSystemManager::generateDefaultSystems(void){
	CSystem* l_system;
	CEmulator l_emulator;

	/*"id";"name";"description";"icon_file";"dat_file";"bios_file";"home_dir";"rom_dir";
"snap_dir";"tittle_dir";"front_dir";"back_dir";"media_dir";emulator_id;
"datname";"datDescription";"datcategory";datversion";"datauthor";datgames;
*/

	/*
	 *  Amstrad - GX4000
	 */
	l_system = new CSystem();
	l_system->setName(_("Amstrad GX4000"));
	l_system->setDescription(_("Console released by Amstrad in 1990 and was based on the still-popular CPC technology."));
	l_system->setIconFile(GELIDE_PIXMAPS"amstrad-gx4000.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("gx4000 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("gx4000 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Atari - 2600
	 */
	l_system = new CSystem();
	l_system->setName(_("Atari 2600"));
	l_system->setDescription(_("Originally known as the Atari VCS, was released in October 1977"));
	l_system->setIconFile(GELIDE_PIXMAPS"atari-2600.png");
	// Emulador Stella
	l_emulator.setId(0);
	l_emulator.setName(_("Stella"));
	l_emulator.setDescription(_("A multi-platform Atari VCS 2600 emulator"));
	l_emulator.setVersion("3.2.2");
	l_emulator.setAuthor(_("Bradford W. Mott & The Stella Team."));
	l_emulator.setHomepage("http://stella.sourceforge.net");
	l_emulator.setPath("/usr/bin/stella");
	l_emulator.setParams("-video gl -gl_fsmax 1 -gl_filter nearest -gl_aspect 1.7 -fullscreen 1 -center 1 -sound 1 -channels 2 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"stella.png");
	l_system->addEmulator(l_emulator);
	// Emulador z26
	l_emulator.setId(0);
	l_emulator.setName(_("z26"));
	l_emulator.setDescription(_("An Atari 2600 Emulator"));
	l_emulator.setVersion("2.13");
	l_emulator.setAuthor(_("John Saeger."));
	l_emulator.setHomepage("http://www.whimsey.com/z26/z26.html");
	l_emulator.setPath("/usr/bin/z26");
	l_emulator.setParams("-v8 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"z26.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("a2600 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("a2600 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("a2600 -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Atari - 5200
	 */
	l_system = new CSystem();
	l_system->setName(_("Atari 5200"));
	l_system->setDescription(_("Atari 5200 SuperSystem was the replacement for the famous Atari 2600"));
	l_system->setIconFile(GELIDE_PIXMAPS"atari-5200.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("a5200 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("a5200 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("a5200 -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Atari - 7800
	 */
	l_system = new CSystem();
	l_system->setName(_("Atari 7800"));
	l_system->setDescription(_("Video game console released by Atari Corp in June 1986"));
	l_system->setIconFile(GELIDE_PIXMAPS"atari-7800.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("a7800 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("a7800 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("a7800p -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Atari - Jaguar
	 */
	l_system = new CSystem();
	l_system->setName(_("Atari Jaguar"));
	l_system->setDescription(_("The first 64-Bits gaming system released by Atari Corp. in 1993"));
	l_system->setIconFile(GELIDE_PIXMAPS"atari-jaguar.png");
	// Emulador Virtual Jaguar Gcc/Sdl
	l_emulator.setId(0);
	l_emulator.setName(_("Virtual Jaguar GCC/SDL"));
	l_emulator.setDescription(_("Portable emulator based in Virtual Jaguar"));
	l_emulator.setVersion("1.0.7");
	l_emulator.setAuthor(_("Niels Wagenaar, Carwin Jones, James L. Hammons and Adam Gree"));
	l_emulator.setHomepage("http://sdlemu.ngemu.com/");
	l_emulator.setPath("/usr/bin/virtualjaguar");
	l_emulator.setParams("\"$gf\" -fullscreen -gl");
	l_emulator.setIconFile(GELIDE_PIXMAPS"virtualjaguar.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("jaguar -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("jaguar -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Atari - Lynx
	 */
	l_system = new CSystem();
	l_system->setName(_("Atari Lynx"));
	l_system->setDescription(_("Was a handheld game console released by Atari Corporation in 1989"));
	l_system->setIconFile(GELIDE_PIXMAPS"atari-lynx.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-lynx.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador handysdl
	l_emulator.setId(0);
	l_emulator.setName(_("Handy/SDL"));
	l_emulator.setDescription(_("SDL based port of the famous Handy Atari Lynx emulator"));
	l_emulator.setVersion("0.82 R1");
	l_emulator.setAuthor(_("Niels Wagenaar & Caz"));
	l_emulator.setHomepage("http://sdlemu.ngemu.com/handysdl.php");
	l_emulator.setPath("/usr/games/sdlhandy");
	l_emulator.setParams("\"$gf\" -sound -fullscreen");
	l_emulator.setIconFile(GELIDE_PIXMAPS"handysdl.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("lynx -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("lynx -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("lynx -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Bandai - WonderSwan
	 */
	l_system = new CSystem();
	l_system->setName(_("Bandai WonderSwan"));
	l_system->setDescription(_("Handheld 16-Bits game console released in Japan by Bandai in 1999"));
	l_system->setIconFile(GELIDE_PIXMAPS"bandai-wonderswan.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-wswan.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m wonderswan \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("wswan -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("wswan -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("wswan -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Bandai - WonderSwan Color
	 */
	l_system = new CSystem();
	l_system->setName(_("Bandai WonderSwan Color"));
	l_system->setDescription(_("WonderSwan color version released in Japan by Bandai in 2000"));
	l_system->setIconFile(GELIDE_PIXMAPS"bandai-wonderswancolor.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-wswan.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m wonderswan \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("wscolor -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("wscolor -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("wscolor -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Coleco - ColecoVision
	 */
	l_system = new CSystem();
	l_system->setName(_("Coleco ColecoVision"));
	l_system->setDescription(_("Coleco Industries' second generation video game console released in August 1982"));
	l_system->setIconFile(GELIDE_PIXMAPS"coleco-colecovision.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("coleco -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("coleco -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("coleco -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Emerson - Arcadia 2001
	 */
	l_system = new CSystem();
	l_system->setName(_("Emerson Arcadia 2001"));
	l_system->setDescription(_("A 8-Bits Second generation video game console"));
	l_system->setIconFile(GELIDE_PIXMAPS"emerson-arcadia2001.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("arcadia -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("arcadia -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("arcadia -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Entex - Adventure Vision
	 */
	l_system = new CSystem();
	l_system->setName(_("Entex Adventure Vision"));
	l_system->setDescription(_("Entex's second generation video game console"));
	l_system->setIconFile(GELIDE_PIXMAPS"entex-adventurevision.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("advision -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("advision -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("advision -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Fairchild - Channel F
	 */
	l_system = new CSystem();
	l_system->setName(_("Fairchild Channel F"));
	l_system->setDescription(_("Game console released by Fairchild in 1976. It was the first programmable ROM cartridge console."));
	l_system->setIconFile(GELIDE_PIXMAPS"fairchild-channelf.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("channelf -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("channelf -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("channelf -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  GCE - Vectrex
	 */
	l_system = new CSystem();
	l_system->setName(_("GCE Vectrex"));
	l_system->setDescription(_("8-Bits console with an integrated vector monitor which displays vector graphics"));
	l_system->setIconFile(GELIDE_PIXMAPS"gce-vectrex.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("vectrex -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("vectrex -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("vectrex -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Magnavox - Odyssey2
	 */
	l_system = new CSystem();
	l_system->setName(_("Magnavox Odyssey2"));
	l_system->setDescription(_("Magnavox second generation video game console released in 1978"));
	l_system->setIconFile(GELIDE_PIXMAPS"magnavox-odyssey2.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("odyssey2 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("odyssey2 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("odyssey2 -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Mame: Multiple Arcade Machine Emulator
	 */
	l_system = new CSystem();
	l_system->setName(_("Mame"));
	l_system->setDescription(_("Multiple Arcade Machine Emulator"));
	l_system->setIconFile(GELIDE_PIXMAPS"arcade-mame.png");
	// Emulador MAME
	l_emulator.setId(0);
	l_emulator.setName(_("Mame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("Nicola Salmoria and the Mame Team"));
	l_emulator.setHomepage("http://www.mamedev.org/");
	l_emulator.setPath("/usr/games/mame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mame.png");
	l_system->addEmulator(l_emulator);
	// Emulador SdlMame
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMame"));
	l_emulator.setDescription(_("Portable MAME version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert, R. Belmont and the MAME Team"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmame.png");
	l_system->addEmulator(l_emulator);
	// Emulador XMame
	l_emulator.setId(0);
	l_emulator.setName(_("XMame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame Team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmame");
	l_emulator.setParams("-vidmod 0 -fullscreen -ef 1 -aa -ka -rp \"$gp\" -skip_gameinfo \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmame.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Mattel - Intellivision
	 */
	l_system = new CSystem();
	l_system->setName(_("Mattel Intellivision"));
	l_system->setDescription(_("Video game console released by Mattel Electronics in 1979"));
	l_system->setIconFile(GELIDE_PIXMAPS"mattel-intellivision.png");
	// Emulador jzIntv
	l_emulator.setId(0);
	l_emulator.setName(_("jzIntv"));
	l_emulator.setDescription(_("Joe Zbiciak's Intellivision Emulator for Linux"));
	l_emulator.setVersion("1.0-beta3");
	l_emulator.setAuthor(_("Joe Zbiciak"));
	l_emulator.setHomepage("http://spatula-city.org/~im14u2c/intv");
	l_emulator.setPath("/usr/games/jzintv");
	l_emulator.setParams("-e \"$bp/exec.bin\" -g \"$bp/grom.bin\" -z1 -f1 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"jzintv.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("intv -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("intv -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("intv -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  NEC - Pc Engine
	 */
	l_system = new CSystem();
	l_system->setName(_("NEC Pc Engine"));
	l_system->setDescription(_("8-Bits video game console also known as TurboGrafx "));
	l_system->setIconFile(GELIDE_PIXMAPS"nec-pcengine.png");
	// Emulador Hu-Go
	l_emulator.setId(0);
	l_emulator.setName(_("Hu-Go!"));
	l_emulator.setDescription(_("Multiplatform Pc Engine / TurboGrafx emulator"));
	l_emulator.setVersion("2.12");
	l_emulator.setAuthor(_("Zeograd"));
	l_emulator.setHomepage("http://www.zeograd.com");
	l_emulator.setPath("/usr/bin/hugo");
	l_emulator.setParams("-a -f -s \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"hu-go.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-pce.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("tg16 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("tg16 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("tg16 -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  NEC - SuperGrafx
	 */
	l_system = new CSystem();
	l_system->setName(_("NEC SuperGrafx"));
	l_system->setDescription(_("8-Bits video game console successor of the Pc Engine"));
	l_system->setIconFile(GELIDE_PIXMAPS"nec-supergrafx.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-pce.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("sgx -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("sgx -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - DS
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo DS"));
	l_system->setDescription(_("Dual-screen handheld console released by Nintendo in 2004"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-ds.png");
	// Emulador Gambatte
	l_emulator.setId(0);
	l_emulator.setName(_("DeSmuME"));
	l_emulator.setDescription(_("A Nintendo DS emulator under GNU GPL"));
	l_emulator.setVersion("0.9.6");
	l_emulator.setAuthor(_("Yopyop and The DeSmuME team"));
	l_emulator.setHomepage("http://desmume.org/");
	l_emulator.setPath("/usr/local/bin/desmume-cli");
	l_emulator.setParams("--auto-pause --opengl-2d \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"desmume.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - Famicom Disk System
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo Famicom Disk System"));
	l_system->setDescription(_("Was released in 1986 by Nintendo as a peripheral for the Famicom console"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-famicom.png");
	// Emulador FCEUX
	l_emulator.setId(0);
	l_emulator.setName(_("FCEUX"));
	l_emulator.setDescription(_("A Evolution of the original FCE Ultra emulator"));
	l_emulator.setVersion("2.1.4a");
	l_emulator.setAuthor(_("The FCEUX team"));
	l_emulator.setHomepage("http://www.fceux.com");
	l_emulator.setPath("/usr/local/bin/fceux");
	l_emulator.setParams("--xres 800 --yres 600 --autoscale 1 --keepratio 1 --opengl 1 --doublebuf 1 --fullscreen 1 --sound 1 --volume 50 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"fceux.png");
	l_system->addEmulator(l_emulator);
	// Emulador Nestopia
	l_emulator.setId(0);
	l_emulator.setName(_("Nestopia"));
	l_emulator.setDescription(_("Open source NES/Famicom emulator focused accuracy"));
	l_emulator.setVersion("1.4.0");
	l_emulator.setAuthor(_("Martin Freij & R. Belmont (Linux port)"));
	l_emulator.setHomepage("http://nestopia.sourceforge.net/");
	l_emulator.setPath("/usr/games/nestopia");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"nestopia.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-nes.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m famicom \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("famicom -keepaspect -skip_gameinfo -sound -multithreading -video opengl -flop \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("famicom -keepaspect -skip_gameinfo -sound -multithreading -video opengl -flop \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - Game boy
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo Game Boy"));
	l_system->setDescription(_("Handheld video game console developed by Nintendo released in 1989"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-gameboy.png");
	// Emulador Gambatte
	l_emulator.setId(0);
	l_emulator.setName(_("Gambatte"));
	l_emulator.setDescription(_("Accuracy-focused, cross-platform Game Boy Color emulator"));
	l_emulator.setVersion("0.4.1");
	l_emulator.setAuthor(_("Sindre Aamås"));
	l_emulator.setHomepage("https://sourceforge.net/projects/gambatte/");
	l_emulator.setPath("/usr/games/gambatte");
	l_emulator.setParams("-f \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gambatte.png");
	l_system->addEmulator(l_emulator);
	// Emulador Gngb
	l_emulator.setId(0);
	l_emulator.setName(_("Gngb"));
	l_emulator.setDescription(_("A Game Boy (Color) emulator for GNU/Linux written in C using SDL"));
	l_emulator.setVersion("20060309");
	l_emulator.setAuthor(_("Frogus & Pepone"));
	l_emulator.setHomepage("http://m.peponas.free.fr/gngb/index.html");
	l_emulator.setPath("/usr/bin/gngb");
	l_emulator.setParams("-a -o -s -f \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gngb.png");
	l_system->addEmulator(l_emulator);
	// Emulador Gnuboy
	l_emulator.setId(0);
	l_emulator.setName(_("Gnuboy"));
	l_emulator.setDescription(_("One of the few Game Boy (Color) emulator for GNU/Linux"));
	l_emulator.setVersion("1.0.3");
	l_emulator.setAuthor(_("Laguna and Gilgamesh"));
	l_emulator.setHomepage("http://gnuboy.unix-fu.org/");
	l_emulator.setPath("/usr/games/sdlgnuboy");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gnuboy.png");
	l_system->addEmulator(l_emulator);
	// Emulador VisualBoy Advance
	l_emulator.setId(0);
	l_emulator.setName(_("VisualBoy Advance"));
	l_emulator.setDescription(_("VBA is one of the most known emulators for GBA"));
	l_emulator.setVersion("1.8.0");
	l_emulator.setAuthor(_("Forgotten and The VBA Team."));
	l_emulator.setHomepage("http://vba.ngemu.com");
	l_emulator.setPath("/usr/bin/vba");
	l_emulator.setParams("-F -f 7 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"visualboyadvance.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-gb.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m gameboy \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("gameboy -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("gameboy -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("gameboy -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - Game Boy Color
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo Game Boy Color"));
	l_system->setDescription(_("Successor of the Game Boy released in October 1998"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-gameboycolor.png");
	// Emulador Gambatte
	l_emulator.setId(0);
	l_emulator.setName(_("Gambatte"));
	l_emulator.setDescription(_("Accuracy-focused, cross-platform Game Boy Color emulator"));
	l_emulator.setVersion("0.4.1");
	l_emulator.setAuthor(_("Sindre Aamås"));
	l_emulator.setHomepage("https://sourceforge.net/projects/gambatte/");
	l_emulator.setPath("/usr/games/gambatte");
	l_emulator.setParams("-f \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gambatte.png");
	l_system->addEmulator(l_emulator);
	// Emulador Gngb
	l_emulator.setId(0);
	l_emulator.setName(_("Gngb"));
	l_emulator.setDescription(_("A Game Boy (Color) emulator for GNU/Linux written in C using SDL"));
	l_emulator.setVersion("20060309");
	l_emulator.setAuthor(_("Frogus & Pepone"));
	l_emulator.setHomepage("http://m.peponas.free.fr/gngb/index.html");
	l_emulator.setPath("/usr/bin/gngb");
	l_emulator.setParams("-a -o -s -f \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gngb.png");
	l_system->addEmulator(l_emulator);
	// Emulador Gnuboy
	l_emulator.setId(0);
	l_emulator.setName(_("Gnuboy"));
	l_emulator.setDescription(_("One of the few Game Boy (Color) emulator for GNU/Linux"));
	l_emulator.setVersion("1.0.3");
	l_emulator.setAuthor(_("Laguna and Gilgamesh"));
	l_emulator.setHomepage("http://gnuboy.unix-fu.org/");
	l_emulator.setPath("/usr/games/sdlgnuboy");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gnuboy.png");
	l_system->addEmulator(l_emulator);
	// Emulador VisualBoy Advance
	l_emulator.setId(0);
	l_emulator.setName(_("VisualBoy Advance"));
	l_emulator.setDescription(_("VBA is one of the most known emulators for GBA"));
	l_emulator.setVersion("1.8.0");
	l_emulator.setAuthor(_("Forgotten and The VBA Team."));
	l_emulator.setHomepage("http://vba.ngemu.com");
	l_emulator.setPath("/usr/bin/vba");
	l_emulator.setParams("-F -f 7 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"visualboyadvance.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-gb.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m gameboy \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("gbcolor -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("gbcolor -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("gbcolor -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - Game Boy Advance
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo Game Boy Advance"));
	l_system->setDescription(_("32-Bit handheld video game console developed by Nintendo released in 2001"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-gameboyadvance.png");
	// Emulador VisualBoy Advance
	l_emulator.setId(0);
	l_emulator.setName(_("VisualBoy Advance"));
	l_emulator.setDescription(_("VBA is one of the most known emulators for GBA"));
	l_emulator.setVersion("1.8.0");
	l_emulator.setAuthor(_("Forgotten and The VBA Team."));
	l_emulator.setHomepage("http://vba.ngemu.com");
	l_emulator.setPath("/usr/bin/vba");
	l_emulator.setParams("-F -f 7 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"visualboyadvance.png");
	l_system->addEmulator(l_emulator);
	// Emulador BoyCott Advance-SDL
	l_emulator.setId(0);
	l_emulator.setName(_("BoyCott Advance-SDL"));
	l_emulator.setDescription(_("A freeware Nintendo Game Boy Advance emulator."));
	l_emulator.setVersion("0.2.8 R1");
	l_emulator.setAuthor(_("SDLEmu development-team"));
	l_emulator.setHomepage("http://sdlemu.ngemu.com/basdl.php");
	l_emulator.setPath("/usr/games/boyca");
	l_emulator.setParams("\"$gf\" -sound -opengl -fullscreen");
	l_emulator.setIconFile(GELIDE_PIXMAPS"boycottadvance-sdl.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-gba.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m gbadvance -m gbadvance \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("gba -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("gba -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - NES
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo NES"));
	l_system->setDescription(_("8-Bit video game console developed by Nintendo and released in 1983"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-nes.png");
	// Emulador FCEUX
	l_emulator.setId(0);
	l_emulator.setName(_("FCEUX"));
	l_emulator.setDescription(_("A Evolution of the original FCE Ultra emulator"));
	l_emulator.setVersion("2.1.4a");
	l_emulator.setAuthor(_("The FCEUX team"));
	l_emulator.setHomepage("http://www.fceux.com");
	l_emulator.setPath("/usr/local/bin/fceux");
	l_emulator.setParams("--xres 800 --yres 600 --autoscale 1 --keepratio 1 --opengl 1 --doublebuf 1 --fullscreen 1 --sound 1 --volume 50 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"fceux.png");
	l_system->addEmulator(l_emulator);
	// Emulador Nestopia
	l_emulator.setId(0);
	l_emulator.setName(_("Nestopia"));
	l_emulator.setDescription(_("Open source NES/Famicom emulator focused accuracy"));
	l_emulator.setVersion("1.4.0");
	l_emulator.setAuthor(_("Martin Freij & R. Belmont (Linux port)"));
	l_emulator.setHomepage("http://nestopia.sourceforge.net/");
	l_emulator.setPath("/usr/games/nestopia");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"nestopia.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-nes.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m famicom \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("nes -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("nes -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("nes -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - SNes
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo SNES"));
	l_system->setDescription(_("16-Bits video game console sucessor of the Nes"));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-snes.png");
	// Emulador Snes9x-Gtk
	l_emulator.setId(0);
	l_emulator.setName(_("Snes9x-gtk"));
	l_emulator.setDescription(_("Gtk+ port of Snes9x Free SNes emulator"));
	l_emulator.setVersion("1.52.79");
	l_emulator.setAuthor(_("Brandon Wright (bearoso)"));
	l_emulator.setHomepage("http://code.google.com/p/snes9x-gtk/");
	l_emulator.setPath("/usr/local/bin/snes9x-gtk");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"snes9x-gtk.png");
	l_system->addEmulator(l_emulator);
	// Emulador bsnes
	l_emulator.setId(0);
	l_emulator.setName(_("Bsnes"));
	l_emulator.setDescription(_("An accuracy focused emulator for Nintendo SNES"));
	l_emulator.setVersion("070");
	l_emulator.setAuthor(_("Byuu"));
	l_emulator.setHomepage("http://byuu.org/bsnes/");
	l_emulator.setPath("/usr/local/bin/bsnes");
	l_emulator.setParams("\"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"bsnes.png");
	l_system->addEmulator(l_emulator);
	// Emulador Zsnes
	l_emulator.setId(0);
	l_emulator.setName(_("Zsnes"));
	l_emulator.setDescription(_("Zsnes - A Super Nintendo emulator"));
	l_emulator.setVersion("1.51");
	l_emulator.setAuthor(_("zsKnight & _Demo_"));
	l_emulator.setHomepage("http://www.zsnes.com");
	l_emulator.setPath("/usr/bin/zsnes");
	l_emulator.setParams("-m -s -r 5 -k 75 -v 7 -ad sdl \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"zsnes.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m superfamicom \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("snes -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("snes -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("snes -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Nintendo - 64
	 */
	l_system = new CSystem();
	l_system->setName(_("Nintendo 64"));
	l_system->setDescription(_("64-Bit console released by Nintendo in 1996 in Japan."));
	l_system->setIconFile(GELIDE_PIXMAPS"nintendo-n64.png");
	// Emulador Mupen64plus
	l_emulator.setId(0);
	l_emulator.setName(_("Mupen64Plus"));
	l_emulator.setDescription(_("An open source Nintendo 64 emulator"));
	l_emulator.setVersion("1.5");
	l_emulator.setAuthor(_("The Mupen64Plus Team"));
	l_emulator.setHomepage("http://code.google.com/p/mupen64plus");
	l_emulator.setPath("/usr/local/bin/mupen64plus");
	l_emulator.setParams("--nogui --noask --noosd --fullscreen \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mupen64plus.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("n64 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("n64 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Raine: Rainbow Island Emulator
	 */
	l_system = new CSystem();
	l_system->setName(_("Raine"));
	l_system->setDescription(_("M680x0 Arcade Emulation"));
	l_system->setIconFile(GELIDE_PIXMAPS"arcade-raine.png");
	// Emulador raine
	l_emulator.setId(0);
	l_emulator.setName(_("Raine"));
	l_emulator.setDescription(_("Multi arcade emulator mainly focused on Taito and Jaleco hardware"));
	l_emulator.setVersion("0.51.7");
	l_emulator.setAuthor(_("Antiriad And Raine Team"));
	l_emulator.setHomepage("http://rainemu.swishparty.co.uk/");
	l_emulator.setPath("/usr/games/raine");
	l_emulator.setParams("-nogui -game $gn");
	l_emulator.setIconFile(GELIDE_PIXMAPS"raine.png");
	l_system->addEmulator(l_emulator);
	// Emulador MAME
	l_emulator.setId(0);
	l_emulator.setName(_("Mame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("Nicola Salmoria and the Mame Team"));
	l_emulator.setHomepage("http://www.mamedev.org/");
	l_emulator.setPath("/usr/games/mame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mame.png");
	l_system->addEmulator(l_emulator);
	// Emulador SdlMame
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMame"));
	l_emulator.setDescription(_("Portable MAME version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert, R. Belmont and the MAME Team"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmame.png");
	l_system->addEmulator(l_emulator);
	// Emulador XMame
	l_emulator.setId(0);
	l_emulator.setName(_("XMame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame Team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmame");
	l_emulator.setParams("-vidmod 0 -fullscreen -ef 1 -aa -ka -doublebuf -rp \"$gp\" -skip_gameinfo \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmame.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  RCA - Studio II
	 */
	l_system = new CSystem();
	l_system->setName(_("RCA Studio II"));
	l_system->setDescription(_("Video game console made by RCA that debuted in January 1977"));
	l_system->setIconFile(GELIDE_PIXMAPS"rca-studio2.png");
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("studio2 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("studio2 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador xmess
	l_emulator.setId(0);
	l_emulator.setName(_("XMess"));
	l_emulator.setDescription(_("Multiple Emulator Super System for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmess");
	l_emulator.setParams("studio2 -fullscreen -ef 1 -ka -skip_warnings -skip_gameinfo -doublebuf -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SEGA - 32X
	 */
	l_system = new CSystem();
	l_system->setName(_("SEGA 32X"));
	l_system->setDescription(_("Add-on for the Mega Drive/Genesis who adds support for 32bits"));
	l_system->setIconFile(GELIDE_PIXMAPS"sega-32x.png");
	// Emulador gens/gs
	l_emulator.setId(0);
	l_emulator.setName(_("Gens/GS"));
	l_emulator.setDescription(_("Free SEGA Mega Drive, 32X and SEGA CD emulator based on Gens"));
	l_emulator.setVersion("2.16.7 (gs-r7)");
	l_emulator.setAuthor(_("Stephane Dallongeville\nStephane Akhoun\nDavid Korth (GS)"));
	l_emulator.setHomepage("http://www.segaretro.org/Gens/GS");
	l_emulator.setPath("/usr/bin/gens");
	l_emulator.setParams("--fs --disable-fps \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gens.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("32x -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("32x -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SEGA - Game Gear
	 */
	l_system = new CSystem();
	l_system->setName(_("SEGA Game Gear"));
	l_system->setDescription(_("Handheld video games system released by SEGA as response to Game Boy"));
	l_system->setIconFile(GELIDE_PIXMAPS"sega-gamegear.png");
	// Emulador osmose
	l_emulator.setId(0);
	l_emulator.setName(_("Osmose"));
	l_emulator.setDescription(_("Open source SEGA Master System / Gamegear emulator"));
	l_emulator.setVersion("0.9.2");
	l_emulator.setAuthor(_("Bruno Vedder"));
	l_emulator.setHomepage("http://bcz.emu-france.com/osmose.htm");
	l_emulator.setPath("/usr/games/osmose");
	l_emulator.setParams("-fs -bilinear \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"osmose.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-gg.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m gamegear \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("gamegear -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("gamegear -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SEGA - Master System
	 */
	l_system = new CSystem();
	l_system->setName(_("SEGA Master System"));
	l_system->setDescription(_("8-Bits cartridge based video game console released by SEGA"));
	l_system->setIconFile(GELIDE_PIXMAPS"sega-mastersystem.png");
	// Emulador osmose
	l_emulator.setId(0);
	l_emulator.setName(_("Osmose"));
	l_emulator.setDescription(_("Open source SEGA Master System / Gamegear emulator"));
	l_emulator.setVersion("0.9.2");
	l_emulator.setAuthor(_("Bruno Vedder"));
	l_emulator.setHomepage("http://bcz.emu-france.com/osmose.htm");
	l_emulator.setPath("/usr/games/osmose");
	l_emulator.setParams("-fs -bilinear \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"osmose.png");
	l_system->addEmulator(l_emulator);
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-sms.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m mark3 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("sms -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("sms -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SEGA - Mega Drive
	 */
	l_system = new CSystem();
	l_system->setName(_("SEGA Mega Drive"));
	l_system->setDescription(_("16-Bits video game console developed by SEGA. Also known as Genesis"));
	l_system->setIconFile(GELIDE_PIXMAPS"sega-megadrive.png");
	// Emulador gens/gs
	l_emulator.setId(0);
	l_emulator.setName(_("Gens/GS"));
	l_emulator.setDescription(_("Free SEGA Mega Drive, 32X and SEGA CD emulator based on Gens"));
	l_emulator.setVersion("2.16.7 (gs-r7)");
	l_emulator.setAuthor(_("Stephane Dallongeville\nStephane Akhoun\nDavid Korth (GS)"));
	l_emulator.setHomepage("http://www.segaretro.org/Gens/GS");
	l_emulator.setPath("/usr/bin/gens");
	l_emulator.setParams("--fs --disable-fps \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gens.png");
	l_system->addEmulator(l_emulator);
	// Emulador regen
	l_emulator.setId(0);
	l_emulator.setName(_("Regen"));
	l_emulator.setDescription(_("SEGA Genesis/MegaDrive emulator written for maximum accuracy"));
	l_emulator.setVersion("0.9");
	l_emulator.setAuthor(_("Aamir Maghul"));
	l_emulator.setHomepage("http://aamirm.hacking-cult.org/www/regen.html");
	l_emulator.setPath("/usr/games/regen");
	l_emulator.setParams("-f \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"regen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Generator (cbiere)
	l_emulator.setId(0);
	l_emulator.setName(_("Generator"));
	l_emulator.setDescription(_("Open source SEGA Mega Drive / Genesis emulator"));
	l_emulator.setVersion("0.35.4 (cbiere r4)");
	l_emulator.setAuthor(_("James Ponder, Christian Biere (SDL)"));
	l_emulator.setHomepage("http://www.squish.net/generator/");
	l_emulator.setPath("/usr/local/bin/generator-sdl");
	l_emulator.setParams("-a \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"generator.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m megadrive \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("genesis -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("genesis -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SEGA - SG1000
	 */
	l_system = new CSystem();
	l_system->setName(_("SEGA SG1000"));
	l_system->setDescription(_("Was the first video game console developed by SEGA"));
	l_system->setIconFile(GELIDE_PIXMAPS"sega-sg1000.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-sms.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m sg1000 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("sg1000 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("sg1000 -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  Sinclair - ZX Spectrum
	 */
	l_system = new CSystem();
	l_system->setName(_("Sinclair ZX Spectrum"));
	l_system->setDescription(_("An 8-bit personal home computer released in 1982 by Sinclair Research Ltd"));
	l_system->setIconFile(GELIDE_PIXMAPS"sinclair-zxspectrum.png");
	// Emulador fbzx
	l_emulator.setId(0);
	l_emulator.setName(_("FBZX"));
	l_emulator.setDescription(_("A ZX Spectrum emulator for FrameBuffer"));
	l_emulator.setVersion("2.4.2");
	l_emulator.setAuthor(_("Raster Software Vigo (Sergio Costas)"));
	l_emulator.setHomepage("http://www.rastersoft.com/fbzx.html");
	l_emulator.setPath("/usr/bin/fbzx");
	l_emulator.setParams("-fs -db -ds \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"fbzx.png");
	l_system->addEmulator(l_emulator);
	// Emulador fuse
	l_emulator.setId(0);
	l_emulator.setName(_("Fuse"));
	l_emulator.setDescription(_("The Free Unix Spectrum Emulator (Fuse)"));
	l_emulator.setVersion("1.0.0.1");
	l_emulator.setAuthor(_("Philip Kendall"));
	l_emulator.setHomepage("http://fuse-emulator.sourceforge.net");
	l_emulator.setPath("/usr/bin/fuse-sdl");
	l_emulator.setParams("--kempston --full-screen --graphics-filter tv2x \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"fuse.png");
	l_system->addEmulator(l_emulator);	
	// Emulador spectemu
	l_emulator.setId(0);
	l_emulator.setName(_("SpectEmu"));
	l_emulator.setDescription(_("A 48k ZX-Spectrum emulator for Linux and other UNIX"));
	l_emulator.setVersion("0.94");
	l_emulator.setAuthor(_("Miklos Szeredi"));
	l_emulator.setHomepage("http://www.inf.bme.hu/~mszeredi/spectemu");
	l_emulator.setPath("/usr/bin/xspect");
	l_emulator.setParams("-quick-load \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"spectemu.png");
	l_system->addEmulator(l_emulator);		
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess (Tapes)"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("spec128 -noreadconfig -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cass \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess (Tapes)"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("spec128 -noreadconfig -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cass \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess (Snaps)"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("spec128 -noreadconfig -keepaspect -skip_gameinfo -sound -multithreading -video opengl -dump \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess (Snaps)"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("spec128 -noreadconfig -keepaspect -skip_gameinfo -sound -multithreading -video opengl -dump \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SNK - Neo Geo AES
	 */
	l_system = new CSystem();
	l_system->setName(_("SNK Neo Geo AES"));
	l_system->setDescription(_("Arcade cartridge-based video game system released in 1990 by SNK"));
	l_system->setIconFile(GELIDE_PIXMAPS"snk-neogeo.png");
	// Emulador MAME
	l_emulator.setId(0);
	l_emulator.setName(_("Mame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("Nicola Salmoria and the Mame Team"));
	l_emulator.setHomepage("http://www.mamedev.org/");
	l_emulator.setPath("/usr/games/mame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mame.png");
	l_system->addEmulator(l_emulator);
	// Emulador SdlMame
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMame"));
	l_emulator.setDescription(_("Portable MAME version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert, R. Belmont and the MAME Team"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmame.png");
	l_system->addEmulator(l_emulator);
	// Emulador GnGeo
	l_emulator.setId(0);
	l_emulator.setName(_("GnGeo"));
	l_emulator.setDescription(_("A powerful and fast Neo Geo emulator for GNU/Linux"));
	l_emulator.setVersion("0.7");
	l_emulator.setAuthor(_("Mathieu Peponas & others"));
	l_emulator.setHomepage("http://gngeo.berlios.de/");
	l_emulator.setPath("/usr/bin/gngeo");
	l_emulator.setParams("--biospath=\"$bp\" -f --sound \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"gngeo.png");
	l_system->addEmulator(l_emulator);
	// Emulador XMame
	l_emulator.setId(0);
	l_emulator.setName(_("XMame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame Team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmame");
	l_emulator.setParams("-vidmod 0 -fullscreen -ef 1 -aa -ka -rp \"$gp\" -skip_gameinfo \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmame.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SNK - Neo Geo Pocket
	 */
	l_system = new CSystem();
	l_system->setName(_("SNK Neo Geo Pocket"));
	l_system->setDescription(_("First handheld video games system released by SNK"));
	l_system->setIconFile(GELIDE_PIXMAPS"snk-neogeopocket.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-ngp.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m neopocket \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("ngp -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("ngp -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  SNK - Neo Geo Pocket Color
	 */
	l_system = new CSystem();
	l_system->setName(_("SNK Neo Geo Pocket Color"));
	l_system->setDescription(_("16-Bit color handheld video games system released by SNK"));
	l_system->setIconFile(GELIDE_PIXMAPS"snk-neogeopocketcolor.png");
	// Emulador mednafen
	l_emulator.setId(0);
	l_emulator.setName(_("Mednafen"));
	l_emulator.setDescription(_("Portable Multi-system emulator under GPL"));
	l_emulator.setVersion("0.8.D");
	l_emulator.setAuthor(_("The Mednafen team"));
	l_emulator.setHomepage("http://mednafen.sourceforge.net");
	l_emulator.setPath("/usr/games/mednafen");
	l_emulator.setParams("-ngp.stretch 0 -fs 1 -vdriver 0 \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mednafen.png");
	l_system->addEmulator(l_emulator);
	// Emulador Xe
	l_emulator.setId(0);
	l_emulator.setName(_("Xe Emulator"));
	l_emulator.setDescription(_("Freeware Multi-system emulator for Linux and Windows"));
	l_emulator.setVersion("2.16.2");
	l_emulator.setAuthor(_("James Li"));
	l_emulator.setHomepage("http://www.xe-emulator.com");
	l_emulator.setPath("/usr/local/bin/xe");
	l_emulator.setParams("--lite --fullscreen --render 1 -m neopocket \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xe.png");
	l_system->addEmulator(l_emulator);
	// Emulador mess
	l_emulator.setId(0);
	l_emulator.setName(_("Mess"));
	l_emulator.setDescription(_("Multi Emulator Super System, the sister project of MAME"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("The MESS Team"));
	l_emulator.setHomepage("http://www.mess.org/");
	l_emulator.setPath("/usr/games/mess");
	l_emulator.setParams("ngpc -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mess.png");
	l_system->addEmulator(l_emulator);
	// Emulador sdlmess
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMess"));
	l_emulator.setDescription(_("Portable Mess version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert and R. Belmont"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmess");
	l_emulator.setParams("ngpc -keepaspect -skip_gameinfo -sound -multithreading -video opengl -cart \"$gf\" -bp \"$bp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmess.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

	/*
	 *  ZiNc: 3d arcade emulator
	 */
	l_system = new CSystem();
	l_system->setName(_("ZiNc"));
	l_system->setDescription(_("Zinc - 3d arcade emulator"));
	l_system->setIconFile(GELIDE_PIXMAPS"arcade-zinc.png");
	// Emulador ZiNc
	l_emulator.setId(0);
	l_emulator.setName(_("ZiNc"));
	l_emulator.setDescription(_("Emulator for Sony ZN-1 and ZN-2 and Namco System 11 hardware"));
	l_emulator.setVersion("1.1");
	l_emulator.setAuthor(_("The_Author and DynaChicken"));
	l_emulator.setHomepage("http://www.emuhype.com");
	l_emulator.setPath("/usr/games/zinc");
	l_emulator.setParams("`$ef --list-games|awk '/\\\[$gn/ {print $1}'` --roms-directory=\"$gp\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"zinc.png");
	l_system->addEmulator(l_emulator);
	// Emulador MAME
	l_emulator.setId(0);
	l_emulator.setName(_("Mame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator"));
	l_emulator.setVersion("0.139");
	l_emulator.setAuthor(_("Nicola Salmoria and the Mame Team"));
	l_emulator.setHomepage("http://www.mamedev.org/");
	l_emulator.setPath("/usr/games/mame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"mame.png");
	l_system->addEmulator(l_emulator);
	// Emulador SdlMame
	l_emulator.setId(0);
	l_emulator.setName(_("SdlMame"));
	l_emulator.setDescription(_("Portable MAME version for POSIX-y systems with SDL"));
	l_emulator.setVersion("0.136");
	l_emulator.setAuthor(_("Olivier Galibert, R. Belmont and the MAME Team"));
	l_emulator.setHomepage("http://rbelmont.mameworld.info/?page_id=163");
	l_emulator.setPath("/usr/games/sdlmame");
	l_emulator.setParams("-antialias -sound -skip_gameinfo -multithreading -video opengl -keepaspect -rompath \"$gp\" \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"sdlmame.png");
	l_system->addEmulator(l_emulator);
	// Emulador XMame
	l_emulator.setId(0);
	l_emulator.setName(_("XMame"));
	l_emulator.setDescription(_("Multiple Arcade Machine Emulator for UNIX/Linux"));
	l_emulator.setVersion("0.106");
	l_emulator.setAuthor(_("The XMame Team"));
	l_emulator.setHomepage("http://x.mame.net");
	l_emulator.setPath("/usr/games/xmame");
	l_emulator.setParams("-vidmod 0 -fullscreen -ef 1 -aa -ka -rp \"$gp\" -skip_gameinfo \"$gf\"");
	l_emulator.setIconFile(GELIDE_PIXMAPS"xmame.png");
	l_system->addEmulator(l_emulator);
	addSystem(*l_system);

}

