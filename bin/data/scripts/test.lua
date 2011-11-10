

-- empty Example

frases = {}
font = of.font()
rand = 0.0
visible = 0

function setup()
    of.seedRandom()
    font:loadFont("fonts/verdana.ttf",44)
    frases[0] = "archivo media"
    frases[1] = "archivo 4"
    frases[2] = "todos los archivos"
end

function update()
    --print(ga.getVolume(0))
    if ga.getVolume(0) >= 0.06 then
        if visible == 1 then
            rand = of.random(10)
        end
        visible = 1
    else
        visible = 0
    end
end

function draw()
    ga.background(0.0,1.0)
    of.setColor(255,255*ga.getPitch(0),0)
    of.fill()
    if visible == 1 then
        if rand <= 3 then
            font:drawString(frases[0],20.0,200.0)
        else 
            font:drawString(frases[1],20.0,200.0*2)
        end
    end
end















