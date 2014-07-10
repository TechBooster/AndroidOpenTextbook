#
# Copyright [2014] [Daisuke Miyakawa]
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from helloworld.models import HelloWorldMessage

from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse
from django.http import HttpResponse, HttpResponseRedirect
from django.template import RequestContext, loader

from django.views.decorators.csrf import csrf_exempt

@login_required
def home(request):
    result = HelloWorldMessage.objects.order_by('-created_at')
    if len(result) > 0:
        message = result[0].message
    else:
        message = 'Hello World'

    template = loader.get_template('helloworld/hello.djhtml')
    context = RequestContext(request, {'message': message})
    return HttpResponse(template.render(context))

@login_required
def submit(request):
    message = request.POST.get('message')
    print(message)
    obj = HelloWorldMessage.objects.create(message=message)
    return HttpResponseRedirect(reverse('home'))

