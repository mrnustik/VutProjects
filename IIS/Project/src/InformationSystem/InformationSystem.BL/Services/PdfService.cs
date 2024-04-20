using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using System.Text;
using System.Threading.Tasks;
using DinkToPdf;
using DinkToPdf.Contracts;

namespace InformationSystem.BL.Services
{
    public class PdfService
    {
        private readonly IConverter converter;

        public PdfService(IConverter converter)
        {
            this.converter = converter;
        }

        public byte[] ConvertHtmlToPdf(string html)
        {
            var document = new HtmlToPdfDocument
            {
                GlobalSettings =
                {
                    ColorMode = ColorMode.Color,
                    Orientation = Orientation.Portrait,
                    PaperSize = PaperKind.A4,
                },
                Objects =
                {
                    new ObjectSettings()
                    {
                        PagesCount = true,
                        HtmlContent = html,
                        WebSettings = {DefaultEncoding = "utf-8" }
                    },
                }
            };

            return converter.Convert(document);

        }
    }
}