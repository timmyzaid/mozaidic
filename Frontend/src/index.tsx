import * as React from "react";
import * as ReactDom from "react-dom";
import { Landing } from "./components/Landing";
require("./css/index.css");


ReactDom.render(
    <Landing />,
    document.getElementById("base")
);