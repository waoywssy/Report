var setting = {
    check: {
        enable: true
    },
    async: {
        enable: true,
        url: "http://localhost/drupal/displayfiles",
        autoParam: ["id", "name=n", "level=lv"],
        dataFilter: filter
    }
};

function filter(treeId, parentNode, childNodes) {
    if (!childNodes) return null;
    for (var i = 0, l = childNodes.length; i < l; i++) {
        childNodes[i].name = childNodes[i].name.replace(/\.n/g, '.');
    }
    return childNodes;
}
$(document).ready(function () {
    $.fn.zTree.init($("#treeDemo"), setting);
});