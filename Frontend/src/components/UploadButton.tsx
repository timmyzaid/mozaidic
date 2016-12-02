import * as React from 'react';


export function UploadButton({containerClass, labelClass, buttonText}: UploadButtonProps) {
    return (
        <div className={containerClass}>
            <label htmlFor="file-upload" className={labelClass}>{buttonText}</label>
            <input id="file-upload" type="file"/>
        </div>
    );
}