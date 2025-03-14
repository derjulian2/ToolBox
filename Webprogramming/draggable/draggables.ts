type vec2 =
{
    x: number;
    y: number;
}

function getMousePos(evt : MouseEvent) : vec2
{
    return {x: evt.clientX, y: evt.clientY};
}

function applyDraggable(element: HTMLElement, area: HTMLElement = element) : void
{
    if (element && area)
    {
        element.style.left = element.offsetLeft + "px";
        element.style.top  = element.offsetTop + "px";
    
        area.onmousedown = function(ev: MouseEvent)
        {
            let mouse_down_mpos : vec2 = getMousePos(ev);
            let mouse_down_elempos : vec2 = 
            {
                x: parseInt(element.style.left, 10),
                y: parseInt(element.style.top , 10)
            };

            document.onmouseup = function(_ev: MouseEvent) : void 
            {
                document.onmouseup   = null;
                document.onmousemove = null;
            }

            document.onmousemove = function(_ev: MouseEvent) : void 
            {
                let mouse_move_mpos : vec2 = getMousePos(_ev);
                let delta : vec2 = 
                {
                    x: mouse_down_mpos.x - mouse_move_mpos.x , 
                    y: mouse_down_mpos.y - mouse_move_mpos.y 
                };

                element.style.left = (mouse_down_elempos.x - delta.x) + "px";
                element.style.top  = (mouse_down_elempos.y - delta.y) + "px";
            }
        }
    }
}

function main() : number
{
    let containers : HTMLCollectionOf<Element> = document.getElementsByClassName("container");
    let drag_areas : HTMLCollectionOf<Element> = document.getElementsByClassName("container_header");

    if (containers.length != drag_areas.length)
        return -1;

    for (let i : number = 0; i < containers.length; i++)
    {
        applyDraggable(<HTMLElement>containers[i], <HTMLElement>drag_areas[i]);
    }

    return 0;
}

function invoke_main() : void
{
    console.log("draggables.js exited with code " + main());
}
invoke_main();