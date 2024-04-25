$fn=256;

width=68;
depth=101;
height=24;

padWidth=59;
padDepth=77;
padYOffset=12.72;

yCylHeight = depth + height;
xCylHeight = width + height;

connHousingWidth=34;
connWidth=31;
connHeight=13;

wallThickness=1.5;

module caseOutline() {
    intersection() {
        union() {
            translate([-width/2, yCylHeight/2, 0])
                rotate([90, 0, 0])
                    cylinder(r=height/2, h=yCylHeight);
            translate([width/2, yCylHeight/2, 0])
                rotate([90, 0, 0])
                    cylinder(r=height/2, h=yCylHeight);
            cube([width, yCylHeight, height], center=true);
        }

        union() {
            translate([-xCylHeight/2, -depth/2, 0])
                rotate([0, 90, 0])
                    cylinder(r=height/2, h=xCylHeight);
            translate([-xCylHeight/2, depth/2, 0])
                rotate([0, 90, 0])
                    cylinder(r=height/2, h=xCylHeight);
            cube([xCylHeight, depth, height], center=true);
        }
    }
    translate([0, yCylHeight/2 - height/4, height/4])
        cube([connHousingWidth, height/2, height/2], center=true);
}


intersection() {
    difference() {
        caseOutline();
        union() {
            scale([0.96, 0.96, 0.90])
                caseOutline();
            translate([0, yCylHeight/2, height/2 - connHeight/2 - wallThickness])
                cube([connWidth, 50, connHeight], center=true);
            translate([0, -padYOffset/2, height/2])
                cube([padWidth, padDepth, 10], center=true);
        }
    }
    translate([0, 200 - yCylHeight/2 + height/2 + 6, 0]) 
        cube([400, 400, 400], center=true);
}