
% Create points
a = Point(3.0, 0.0, 0.0);
b = Point(0.0, 3.0, 0.0);

% Specify radius
r = 100;

% Create an instance of the Arc class
arc = Arc(a, b, r);

% Calculate the arc length
arcLength = arc.length();
disp(arcLength);

% Slice the arc with a spacing of 1 mm
spacing = 1.0;  % spacing in mm
slices = arc.sliceArc(spacing);

% Display the slices
disp(slices);
