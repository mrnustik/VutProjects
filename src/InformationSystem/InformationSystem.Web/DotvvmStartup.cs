﻿	using DotVVM.Framework.Configuration;
using DotVVM.Framework.ResourceManagement;
using DotVVM.Framework.Routing;

namespace InformationSystem.Web
{
    public class DotvvmStartup : IDotvvmStartup
    {
        // For more information about this class, visit https://dotvvm.com/docs/tutorials/basics-project-structure
        public void Configure(DotvvmConfiguration config, string applicationPath)
        {
            ConfigureRoutes(config, applicationPath);
            ConfigureControls(config, applicationPath);
            ConfigureResources(config, applicationPath);
        }

        private void ConfigureRoutes(DotvvmConfiguration config, string applicationPath)
        {
            config.RouteTable.Add("Default", "", "Views/default.dothtml");
            config.RouteTable.AutoDiscoverRoutes(new DefaultRouteStrategy(config));    
        }

        private void ConfigureControls(DotvvmConfiguration config, string applicationPath)
        {
            // register code-only controls and markup controls
        }

        private void ConfigureResources(DotvvmConfiguration config, string applicationPath)
        {
            config.Resources.Register("jquery", new ScriptResource(new UrlResourceLocation("https://code.jquery.com/jquery-3.2.1.slim.min.js")));
            config.Resources.Register("bootstrap-js", new ScriptResource(new UrlResourceLocation("https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js")){Dependencies = new []{"jquery"}});
            config.Resources.Register("bootstrap", new StylesheetResource(new UrlResourceLocation("https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css")){Dependencies = new []{"bootstrap-js"}});
            // register custom resources and adjust paths to the built-in resources
        }
    }
}
