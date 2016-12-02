import * as React from 'react';
import { UploadButton } from './UploadButton';
require('../css/landing.css');


export function Landing() {
    return (
        <div className="landing-container">
            <div className="landing-heading">Easily. <span className="italics">Create.</span> Mosaics.</div>
            <div className="landing-sub">Click the button or drag some images to begin.</div>
            <UploadButton buttonText="Upload Images" containerClass="upload-button" labelClass="upload-label"/>
        </div>
    );
}