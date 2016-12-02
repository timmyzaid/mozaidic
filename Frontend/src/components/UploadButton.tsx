import * as React from 'react';


export function UploadButton({containerClass, labelClass, inputClass, buttonText}: UploadButtonProps) {
    return (
        <div className={containerClass}>
            <label htmlFor="file-upload" className={labelClass}>{buttonText}</label>
            <input id="file-upload" className={inputClass} type="file"/>
        </div>
    );
}