// Universal PCB board
// units = mm

// pcb dimentions
thickness = 1.6;
x = 60; // length
y = 80; // width
mountHolesDia = 2.35;
mountHoleOffset = 2;
// margins for pcb holes from PCB edge
xMargin = 3.330;
yMargin = 6.979;

color([0,1,0])
linear_extrude(height = thickness)
difference() {
    square([x,y]);
    {
        // pcb holes
        $fn = 10;
        for(i = [xMargin : 2.54 : x - xMargin],    
            j = [yMargin : 2.54 : y - yMargin]
        ) translate([i,j]) circle(d = 1);

        // mount holes in corners
        $fn=20;
        translate([mountHoleOffset, mountHoleOffset]) circle(d = mountHolesDia);
        translate([mountHoleOffset, y-mountHoleOffset]) circle(d = mountHolesDia);
        translate([x-mountHoleOffset, y-mountHoleOffset]) circle(d = mountHolesDia);
        translate([x-mountHoleOffset, mountHoleOffset]) circle(d = mountHolesDia);
    }
}