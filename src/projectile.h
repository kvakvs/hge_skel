/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */
// to prevent multiple compiling of this header
#pragma once

// Projectile class
// a thrown or shot object with defined speed and straight path
// hits creatures hostile to the shooter, ends its life at solid world objects
//
// Note: in Mario game empty turtle shell might be counted either as a projectile or as a creature
// with changes to its logic
