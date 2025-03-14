function getMousePos(evt) {
    return { x: evt.clientX, y: evt.clientY };
}
function applyDraggable(element, area) {
    if (area === void 0) { area = element; }
    if (element && area) {
        element.style.left = element.offsetLeft + "px";
        element.style.top = element.offsetTop + "px";
        area.onmousedown = function (ev) {
            var mouse_down_mpos = getMousePos(ev);
            var mouse_down_elempos = {
                x: parseInt(element.style.left, 10),
                y: parseInt(element.style.top, 10)
            };
            document.onmouseup = function (_ev) {
                document.onmouseup = null;
                document.onmousemove = null;
            };
            document.onmousemove = function (_ev) {
                var mouse_move_mpos = getMousePos(_ev);
                var delta = {
                    x: mouse_down_mpos.x - mouse_move_mpos.x,
                    y: mouse_down_mpos.y - mouse_move_mpos.y
                };
                element.style.left = (mouse_down_elempos.x - delta.x) + "px";
                element.style.top = (mouse_down_elempos.y - delta.y) + "px";
            };
        };
    }
}
function main() {
    var containers = document.getElementsByClassName("container");
    var drag_areas = document.getElementsByClassName("container_header");
    if (containers.length != drag_areas.length)
        return -1;
    for (var i = 0; i < containers.length; i++) {
        applyDraggable(containers[i], drag_areas[i]);
    }
    return 0;
}
function invoke_main() {
    console.log("draggables.js exited with code " + main());
}
invoke_main();
