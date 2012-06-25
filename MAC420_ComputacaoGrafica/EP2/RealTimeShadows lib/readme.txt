http://www.opengl.org/archives/resources/code/samples/mjktips/rts/index.html  (Real Time Shadows lib)


other links:
http://www.paulsprojects.net/tutorials/smt/smt.html
http://nehe.gamedev.net/tutorial/shadows/16010/
http://www.gamasutra.com/view/feature/131807/realtime_shadow_casting_using_.php?page=3

http://www.gamasutra.com/view/feature/2942/the_mechanics_of_robust_stencil_.php

http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/the-theory-of-stencil-shadow-volumes-r1873


A Clear the frame buffer and perform an ambient rendering pass. Render the visible scene using any surface shading attribute that does not depend on any particular light source.
B Choose a light source and determine what objects may cast shadows into the visible region of the world. If this is not the first light to be rendered, clear the stencil buffer.
C For each object, calculate the silhouette representing the boundary between triangles facing toward the light source and triangles facing away from the light source. Construct a shadow volume by extruding the silhouette away from the light source.
D Render the shadow volume using specific stencil operations that leave nonzero values in the stencil buffer where surfaces are in shadow.
E Perform a lighting pass using the stencil test to mask areas that are not illuminated by the light source.
F Repeat steps B through E for every light source that may illuminate the visible region of the world.
