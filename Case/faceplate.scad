$fn=32;

include <dimensions.scad>

difference() {
    union() {
        translate([0, 0, facePlateTop - facePlateThickness/2])
            difference() {
                union() {
                    // Main plate
                    cube([
                        facePlateWidth + 2 * facePlateOuterMargin,
                        facePlateDepth + 2 * facePlateOuterMargin,
                        facePlateThickness
                    ], center=true);
                    for (x = [0 : facePlateSwitchesX - 2]) {
                        for (y = [0 : facePlateSwitchesY - 2]) {
                            translate([
                                - facePlateWidth/2 + facePlateSwitchOuterMargin +
                                    x * facePlateSwitchDistX + facePlateSwitchHoleWidth +
                                    (facePlateSwitchDistX - facePlateSwitchHoleWidth)/2 ,
                                - facePlateDepth/2 + facePlateSwitchOuterMargin +
                                    y * facePlateSwitchDistY + facePlateSwitchHoleDepth +
                                    (facePlateSwitchDistY - facePlateSwitchHoleDepth)/2,
                                -(facePlateTop - facePlateThickness/2 - 0.01)
                            ])
                                cylinder(r=1, h=facePlateTop);
                        }
                    }
                }
                // Switch holes
                union() {
                    for (x = [0 : facePlateSwitchesX - 1]) {
                        for (y = [0 : facePlateSwitchesY - 1]) {
                            translate([
                                - facePlateWidth/2 + facePlateSwitchOuterMargin +
                                    x * facePlateSwitchDistX + facePlateSwitchHoleWidth/2,
                                - facePlateDepth/2 + facePlateSwitchOuterMargin +
                                    y * facePlateSwitchDistY + facePlateSwitchHoleDepth/2,
                                0
                            ])
                                cube([
                                    facePlateSwitchHoleWidth,
                                    facePlateSwitchHoleDepth,
                                    facePlateThickness + 0.02
                                ], center=true);
                        }
                    }
                }
                // Screw mount hole, bottom left
                translate([
                    -(facePlateWidth + 2 * facePlateOuterMargin)/2 - facePlateDistanceFromBLScrewHoleX,
                    -(facePlateDepth + 2 * facePlateOuterMargin)/2 - facePlateDistanceFromBLScrewHoleY,
                    -5
                ])
                    cylinder(r=3.5, h=10);
            }
        // Rim
        translate([0, -(facePlateDepth/2 - facePlateThickness/2), facePlateTop/2])
            cube([facePlateWidth, facePlateThickness, facePlateTop], center = true);
        translate([-(facePlateWidth/2 - facePlateThickness/2), 0, facePlateTop/2])
            cube([facePlateThickness, facePlateDepth, facePlateTop], center = true);
        translate([(facePlateWidth/2 - facePlateThickness/2), 0, facePlateTop/2])
            cube([facePlateThickness, facePlateDepth, facePlateTop], center = true);
        translate([0, facePlateDepth/2 - facePlateThickness/2, facePlateTop/2])
            cube([facePlateWidth, facePlateThickness, facePlateTop], center=true);

    }
}
