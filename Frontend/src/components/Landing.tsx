import * as React from "react";
require("../css/landing.css");


export function Landing() {
    return (
        <div className="landing-container">
            <div className="landing-heading">Easily. <span className="italics">Create.</span> Mosaics.</div>
            <div className="landing-sub">Click the button or drag some images to begin.</div>
            <div className="landing-button">
                <label htmlFor="file-upload" className="landing-upload">Upload Images</label>
                <input id="file-upload" type="file"/>
            </div>
        </div>
    );
}