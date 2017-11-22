using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.Loader;
using System.Text;
using System.Threading.Tasks;
using DinkToPdf;

namespace InformationSystem.BL.Services
{
    public class PdfService
    {


        internal class CustomAssemblyLoadContext : AssemblyLoadContext
        {
            public IntPtr LoadUnmanagedLibrary(string absolutePath)
            {
                return LoadUnmanagedDll(absolutePath);
            }
            protected override IntPtr LoadUnmanagedDll(String unmanagedDllName)
            {
                return LoadUnmanagedDllFromPath(unmanagedDllName);
            }

            protected override Assembly Load(AssemblyName assemblyName)
            {
                throw new NotImplementedException();
            }
        }

        public byte[] ConvertHtmlToPdf(string html)
        {
            var assemblyLoader = new CustomAssemblyLoadContext();
            assemblyLoader.LoadUnmanagedLibrary(
                "C:\\Development\\University\\IIS\\src\\InformationSystem\\lib\\libwkhtmltox.dll");
            var converter = new BasicConverter(new PdfTools());
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