class 'hLine'

function hLine:__init(y, s)
        self.ypos = y
        self.speed = s
end

function hLine:update()
        self.ypos = self.ypos + self.speed
        if self.ypos > OUTPUT_H then
                self.ypos = 0
        end
end

function hLine:draw()
        of.line(0, self.ypos, OUTPUT_W, self.ypos)
end

---------------------------------------------------
-- create two objects from the class hLine
h1 = hLine(20, 1.7)
h2 = hLine(50, 2.4)
----------------------------------------------------
function setup()
        print("script setup")
end
----------------------------------------------------
function update()
        h1:update()
        h2:update()
end
----------------------------------------------------
function draw()
        ga.background(0.0,1.0)
        of.setColor(255,255,255)
        h1:draw()
        of.setColor(255,255,0)
        h2:draw()
end
