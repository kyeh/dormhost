require 'test_helper'

class RoomProfilesControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:room_profiles)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create room_profile" do
    assert_difference('RoomProfile.count') do
      post :create, :room_profile => { }
    end

    assert_redirected_to room_profile_path(assigns(:room_profile))
  end

  test "should show room_profile" do
    get :show, :id => room_profiles(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => room_profiles(:one).id
    assert_response :success
  end

  test "should update room_profile" do
    put :update, :id => room_profiles(:one).id, :room_profile => { }
    assert_redirected_to room_profile_path(assigns(:room_profile))
  end

  test "should destroy room_profile" do
    assert_difference('RoomProfile.count', -1) do
      delete :destroy, :id => room_profiles(:one).id
    end

    assert_redirected_to room_profiles_path
  end
end
