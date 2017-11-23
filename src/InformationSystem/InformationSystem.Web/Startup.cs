using System;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using DinkToPdf;
using DinkToPdf.Contracts;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.FileProviders;
using Microsoft.Extensions.Logging;
using DotVVM.Framework.Hosting;
using InformationSystem.BL.Services;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;

namespace InformationSystem.Web
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

    public static class WkHtmlToPdf
    {
        public static void Preload()
        {
            var wkHtmlToPdfContext = new CustomAssemblyLoadContext();
            var architectureFolder = (IntPtr.Size == 8) ? "64 bit" : "32 bit";
            var wkHtmlToPdfPath = Path.Combine(AppContext.BaseDirectory, $"wkhtmltox\\{architectureFolder}\\libwkhtmltox");

            wkHtmlToPdfContext.LoadUnmanagedLibrary(wkHtmlToPdfPath);
        }
    }

    public class Startup
    {
        public Startup(IHostingEnvironment env)
        {
            var builder = new ConfigurationBuilder()
                .SetBasePath(env.ContentRootPath)
                .AddJsonFile("appsettings.json", optional: true, reloadOnChange: true);

            Configuration = builder.Build();
        }

        public IConfiguration Configuration { get; }


        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddDataProtection();
            services.AddAuthorization();
            services.AddWebEncoders();
            services.AddEntityFrameworkSqlServer()
                .AddDbContext<ServiceDbContext>(options =>
                {
                    options.UseSqlServer(Configuration.GetConnectionString("DefaultConnection"));
                });
            services.AddIdentity<ApplicationUser, IdentityRole>()
                .AddEntityFrameworkStores<ServiceDbContext>()
                .AddDefaultTokenProviders();

            services.ConfigureApplicationCookie(o =>
            {
                o.Events = new CookieAuthenticationEvents
                {
                    OnRedirectToReturnUrl = c => DotvvmAuthenticationHelper.ApplyRedirectResponse(c.HttpContext, c.RedirectUri),
                    OnRedirectToAccessDenied = c => DotvvmAuthenticationHelper.ApplyStatusCodeResponse(c.HttpContext, 403),
                    OnRedirectToLogin = c => DotvvmAuthenticationHelper.ApplyRedirectResponse(c.HttpContext, c.RedirectUri),
                    OnRedirectToLogout = c => DotvvmAuthenticationHelper.ApplyRedirectResponse(c.HttpContext, c.RedirectUri)
                };
                o.LoginPath = new PathString("/Authentication/SignIn");
            });

            services.Configure<IdentityOptions>(
                o =>
                {
                    o.Password.RequireDigit = false;
                    o.Password.RequireLowercase = false;
                    o.Password.RequiredLength = 5;
                    o.Password.RequireUppercase = false;
                    o.Password.RequireNonAlphanumeric = false;
                });

            services.AddDotVVM(options =>
            {
                options.AddDefaultTempStorages("Temp");
            });
            services.AddTransient<UserService>();
            services.AddTransient<CarService>();
            services.AddTransient<InvoiceService>();
            services.AddTransient<AddressService>();
            services.AddSingleton<PdfService>();
            services.AddTransient<MaterialService>();
            services.AddTransient<RepairService>();
            services.AddTransient<DbIntiliazer>();
            services.AddSingleton(typeof(IConverter), new SynchronizedConverter(new PdfTools()));
            WkHtmlToPdf.Preload();
        }


        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory, DbIntiliazer intiliazer)
        {

            loggerFactory.AddConsole();

            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseAuthentication();

            // use DotVVM
            var dotvvmConfiguration = app.UseDotVVM<DotvvmStartup>(env.ContentRootPath);

            intiliazer.Initialize().Wait();

            // use static files
            app.UseStaticFiles(new StaticFileOptions
            {
                FileProvider = new PhysicalFileProvider(env.WebRootPath)
            });
        }

        public const string AuthenticationScheme = "Cookie";
    }
}
