/*
 * Demo name   : regis_demo
 * Author      : Phillip Stevens @feilipu
 * Version     : V0.2
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <ReGIS.h>

/*
    Expected output (where ^ is ESC character).
    ^P1pS(E)W(I(M))P[600,200]V[][-200,+200]V[][400,100]W(I(G))P[700,100]V(B)[+050,][,+050][-050,](E)V(W(S1))(B)[-100,][,-050][+100,](E)V(W(S1,E))(B)[-050,][,-025][+050,](E)W(I(C))P[200,100]C(A-180)[+100]C(A+180)[+050]W(I(B))P[200,300]C(W(S1))[+100]C(W(S1,E))[+050]W(I(W))T(S02)"hello world"^\
 */

window_t mywindow;

// the setup function runs once when you press reset or power the board
void setup() {

    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }

    Serial.println("Set up complete!");
}

void loop(void)
{

    window_new( &mywindow, 768, 480 );
    window_clear ( &mywindow );

    draw_intensity( &mywindow, _M);

    draw_abs( &mywindow, 600, 200 );
    draw_line_rel( &mywindow, -200, 200 );
    draw_line_abs( &mywindow, 400, 100);

    draw_intensity( &mywindow, _G);

    draw_abs( &mywindow, 700, 100 );
    draw_box( &mywindow, 50, 50);
    draw_box_fill( &mywindow, -100, -50);
    draw_unbox_fill( &mywindow, -50, -25);

    draw_intensity( &mywindow, _C);

    draw_abs( &mywindow, 200, 100 );
    draw_arc( &mywindow, 100, -180);
    draw_arc( &mywindow, 50, 180);

    draw_intensity( &mywindow, _B);

    draw_abs( &mywindow, 200, 300 );
    draw_circle_fill( &mywindow, 100);
    draw_uncircle_fill( &mywindow, 50);

    draw_intensity( &mywindow, _W);

    draw_text( &mywindow, "hello world", 2);

    window_write( &mywindow );
    window_close( &mywindow );

    delay(1000);
}
