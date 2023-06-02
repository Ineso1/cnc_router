% Point class definition
classdef Point
    properties
        x = 0.0;
        y = 0.0;
        z = 0.0;
    end
    
    methods
        function obj = Point(x, y, z)
            if nargin > 0
                obj.x = x;
                obj.y = y;
                obj.z = z;
            end
        end
    end
end